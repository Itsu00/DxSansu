#include <DxLib.h>
#include <vector>
#include "Base.h"
#include "globals.h"
#include "Math2D.h"
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Input.h"
#include "ExplosionEffect.h"

namespace
{
	//Player Parameters：プレイヤーの初期値
	const Vector2D START_POS = { WIN_WIDTH / 2, WIN_HEIGHT / 2 };
	const Vector2D START_VAL = { 0.0f, 0.0f };
	const Vector2D START_DIR = { 0.0f, 1.0f };
	const float START_RADIUS = 30.0f;
	const float START_OMEGA = 2.0f;
	const unsigned int START_COLOR = GetColor(255, 0, 0);
	const unsigned int ENEMY_MAX = 100;//敵の数
	const unsigned int ENEMY_NUM = 8;//最初に出現する敵の数
	Player* player = nullptr;
	std::vector<Bullet*> bullets;//弾丸の保管庫
	std::vector<Enemy*> enemies;//敵の保管庫
	//std::vector<ExplosionEffect*> effects;//エフェクトの保管庫

	std::vector<Base*> objects;//すべてのオブジェクトの保管庫
	void AddObject(Base* obj)
	{
		objects.push_back(obj);
	}

	void UpdateAllObjects()
	{
		for (auto& obj : objects)
		{
			obj->Update();
		}
	}

	void DrawAllObjects()
	{
		for (auto& obj : objects)
		{
			obj->Draw();
		}
	}
}

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	player = new Player(START_POS, START_VAL, START_COLOR,
						START_DIR, START_RADIUS, START_OMEGA);
	AddObject(player);

	enemies.clear();
	enemies.reserve(ENEMY_NUM);

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		int segment = GetRand(8);//8分割
		Enemy* e = new Enemy(Enemy::Size::LARGE, segment);

		//画面外スタート(上から)
		Vector2D pos = {
			(float)GetRand(WIN_WIDTH), -50.0f
		};
		e->SetPos(pos);

		enemies.push_back(e);
		AddObject(e);
	}
}

void Stage::Update()
{
	for (auto& itr : bullets)
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			if (!enemies[i]->IsAlive()) continue;//敵が死んでたらスルー
			float dist = Math2D::Length(Math2D::Sub(itr->GetPos(), enemies[i]->GetPos()));
			if (dist < enemies[i]->GetCollisionRadius())
			{
				//当たった
				enemies[i]->Dead();//敵を消す
				//TODO
				//分裂の処理をここでやりたい
				//大か中か小かを判定して
				//大なら中を2～4つ、中なら小を2～4つ、小なら消える
				Vector2D enemyPos = enemies[i]->GetPos();
				Enemy::Size enemySize = enemies[i]->GetSize();
				if (enemySize == Enemy::Size::SMALL)
				{
					ExplosionEffect* effect = new ExplosionEffect(enemyPos);
					//effects.push_back(effect);
					AddObject(effect);
				}
				else if (enemySize == Enemy::Size::MEDIUM)
				{
					for (int i = 0; i < 4; i++)
					{
						Enemy* e = new Enemy(Enemy::Size::SMALL, 8);
						e->SetPos(enemyPos);
						e->SetVel({ (float)(GetRand(200) - 100), (float)(GetRand(200) - 100) });
						enemies.push_back(e);
						AddObject(e);
					}
				}
				else if (enemySize == Enemy::Size::LARGE)
				{
					for (int i = 0; i < 4; i++)
					{
						Enemy* e = new Enemy(Enemy::Size::MEDIUM, 8);
						e->SetPos(enemyPos);
						e->SetVel({ (float)(GetRand(200) - 100), (float)(GetRand(200) - 100) });
						enemies.push_back(e);
						AddObject(e);
					}
				}
				itr->Dead();//弾を消す
			}
		}
	}

	//賞味期限切れの弾を消す
	DeleteBullet();
	
	//player->Update();
	UpdateAllObjects();

	if (!bullets.empty())
	{
		for (auto& itr : bullets)
		{
			itr->Update();
		}
	}

	/*for (auto& effect : effects)
	{
		effect->Update();
	}*/

	//Zキーが押されたら弾丸を生成
	if (Input::IsKeyDown(KEY_INPUT_Z))
	{
		shootBullet();
	}
}

void Stage::Draw()
{
	DrawAllObjects();
}

void Stage::Release()
{
	delete player;

	for (auto& b : bullets)
	{
		delete b;
	}
	bullets.clear();

	for (auto& e : enemies)
	{
		delete e;
	}
	enemies.clear();
}

void Stage::DeleteBullet()
{
	//賞味期限切れの弾を消す
	for (auto it = bullets.begin(); it != bullets.end();)
	{
		if ((*it)->IsDead() == true)
		{
			it = bullets.erase(it);//弾を消す
		}
		else
		{
			it++;
		}
	}
}

void Stage::shootBullet()
{
	Vector2D pos = player->GetPos();
	Vector2D v = Math2D::Mul(player->GetDirVec(), 300.0f);
	unsigned int bcol = GetColor(255, 255, 255);
	float r = 2;
	float life = 2.0f;

	//Bullet* b = new Bullet(pos, v, GetColor(255, 255, 255), 2, 2.0f);
	Bullet* b = new Bullet(pos, v, bcol, r, life);
	bullets.push_back(b);
	AddObject(b);
}