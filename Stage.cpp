#include "Stage.h"
#include "Math2D.h"
#include "Player.h"
#include "Enemy.h"
#include "globals.h"
#include <DxLib.h>
#include "Bullet.h"
#include "Input.h"
#include <vector>

namespace
{
	//Player Parameters：プレイヤーの初期値
	const Vector2D START_POS = { WIN_WIDTH / 2, WIN_HEIGHT / 2 };
	const Vector2D START_VAL = { 0.0f, 0.0f };
	const Vector2D START_DIR = { 0.0f, 1.0f };
	const float START_RADIUS = 30.0f;
	const float START_OMEGA = 2.0f;
	const unsigned int START_COLOR = GetColor(255, 0, 0);
	Player* player = nullptr;
	std::vector<Bullet*> bullets;//弾丸の保管庫
	std::vector<Enemy*> enemies;//敵の保管庫
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
	//enemy_ = new Enemy(8);

	enemies.clear();
	const int ENEMY_NUM = 8;//敵の数
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		int segment = GetRand(8);//8分割
		Enemy* e = new Enemy(segment);

		//画面外スタート(上から)
		Vector2D pos = {
			(float)GetRand(WIN_WIDTH), -50.0f
		};
		e->SetPos(pos);

		enemies.push_back(e);
	}
}

void Stage::Update()
{
	//賞味期限切れの弾を消す
	//DeleteBullet();
	for (auto it = bullets.begin(); it != bullets.end();)
	{
		if ((*it)->IsDead())// == true
		{
			delete* it;
			it = bullets.erase(it);//弾を消す
		}
		else
		{
			++it;//++it
		}
	}

	player->Update();
	/*if (!bullets.empty())
	{
		for (auto& itr : bullets)
		{
			itr->Update();
		}
	}*/
	for (auto& b : bullets)
	{
		b->Update();
	}
	//Enemy 更新
	for (auto& e : enemies)
	{
		e->Update();
	}

	//Zキーが押されたら弾丸を生成
	if (Input::IsKeyDown(KEY_INPUT_Z))
	{
		Vector2D pos = player->GetPos();
		Vector2D v = Math2D::Mul(player->GetDirVec(), 300.0f);
		/*unsigned int bcol = GetColor(255, 255, 255);
		float r = 2;
		float life = 2.0f;*/
		Bullet* b = new Bullet(pos, v, GetColor(255, 255, 255), 2, 2.0f);
		//Bullet* b = new Bullet(pos, v, bcol, r, life);
		bullets.push_back(b);
	}
}

void Stage::Draw()
{
	/*if (!bullets.empty())
	{
		for (auto& itr : bullets)
		{
			itr->Draw();
		}
	}
	player->Draw();
	enemy_->Draw();*/
	for (auto& b : bullets)
	{
		b->Draw();
	}

	for (auto& e : enemies)
	{
		e->Draw();
	}

	player->Draw();
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