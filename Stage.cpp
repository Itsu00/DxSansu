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
	const float PLAYER_COLLISION_RADIUS = 15.0f;
	const unsigned int ENEMY_MAX = 100;//敵の数
	const unsigned int ENEMY_NUM = 1;//最初に出現する敵の数

	std::vector<Base*> objects;//すべてのオブジェクトの保管庫
	//オブジェクトの保管庫にオブジェクトを追加する関数
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

	int stageState = 0;//0:タイトル 1:プレイ中 2:ゲームオーバー 3:クリア
}

Stage::Stage(){}

Stage::~Stage(){}

void Stage::Initialize()
{
	objects.clear();//オブジェクトの保管庫を空にする

	stageState = 0;//タイトル画面
	
	gameScore_ = 0;

	//変数playerは、ローカル変数なので、この関数が終わると消えてしまう
	//だから、newして動的に確保している
	Player* player = new Player(START_POS, START_VAL, START_COLOR,
						START_DIR, START_RADIUS, START_OMEGA);
	//playerの当たり判定の半径を設定
	player->SetCollisionRadius(PLAYER_COLLISION_RADIUS);
	//オブジェクトの保管庫にplayerを追加
	::AddObject(player);

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		int segment = GetRand(8);//8分割
		Enemy* e = new Enemy(Enemy::Size::LARGE, segment);
		::AddObject(e);
	}

	stars_.clear();
	for (int i = 0; i < 100; i++) { // 星を100個くらい
		Star s;
		s.x = (float)GetRand(WIN_WIDTH);
		s.y = (float)GetRand(WIN_HEIGHT);

		// 真っ白より、少し暗いグレーや青白い色を混ぜるとリアル
		int brightness = GetRand(150) + 100;
		s.color = GetColor(brightness, brightness, brightness);

		stars_.push_back(s);
	}
}

void Stage::Update()
{
	if (stageState == 0) {
		//タイトル画面のアップデート処理
		TitleUpdate();
	}
	else if (stageState == 1) {
		//プレイ画面のアップデート処理
		PlayUpdate();
		if (stageState != 1) return;
	}
	else if (stageState == 2) {
		//ゲームオーバーのアップデート処理
		GameOverUpdate();
	}
	else if (stageState == 3) {
		ClearUpdate();
	}
}

void Stage::Draw()
{
	// 星の描画
	for (const auto& s : stars_) {
		// 半径1の塗りつぶし円。これくらいが星には丁度いいです
		DrawCircle((int)s.x, (int)s.y, 1, s.color, TRUE);
	}

	if (stageState == 0) {
		TitleDraw();
	}
	else if (stageState == 1) {
		PlayDraw();
	}
	else if (stageState == 2) {
		//ゲームオーバーの描画処理
		GameOverDraw();
	}
	else if (stageState == 3) {
		ClearDraw();
	}
}

void Stage::Release(){
	for (auto obj : objects) {
		delete obj;
	}
	objects.clear();
}

void Stage::DeleteBullet()
{
	//賞味期限切れの弾を消す
	//箱の中身を確認→死んでる弾があったらdelete(箱は残るため、nullptrを入れておく)
	for (auto& itr : objects){
		if (itr->GetType() == OBJ_TYPE::BULLET){
			//base->継承クラスの時は、ちゃんと継承クラスのポインタに変換してあげないと、継承クラスのメンバ関数は呼び出せない
			//継承クラス→baseクラスの返還は暗黙的に行われる
			Bullet* b = (Bullet*)(itr);
			if (b->IsDead()){
				delete b;
				itr = nullptr;//ポインタをnullptrにしておく
			}
		}
	}
	//箱の中身を確認→nullptrがあったら箱から消す(箱を詰める)
	for (auto it = objects.begin(); it != objects.end();){
		if (*it == nullptr){
			it = objects.erase(it);
		}
		else{
			it++;
		}
	}
}

void Stage::DeleteEnemy()
{
	for (auto& itr : objects){
		if (itr->GetType() == OBJ_TYPE::ENEMY){
			//base->継承クラスの時は、ちゃんと継承クラスのポインタに変換してあげないと、継承クラスのメンバ関数は呼び出せない
			//継承クラス→baseクラスの返還は暗黙的に行われる
			Enemy* b = (Enemy*)(itr);
			if (b->IsAlive() == false){
				delete b;
				itr = nullptr;//ポインタをnullptrにしておく
			}
		}
	}
	//箱の中身を確認→nullptrがあったら箱から消す(箱を詰める)
	for (auto it = objects.begin(); it != objects.end();){
		if (*it == nullptr){
			it = objects.erase(it);
		}
		else{
			it++;
		}
	}
}

void Stage::DeleteEffect()
{
	//死んでる敵を消す
	for (auto& itr : objects) {
		if (itr->GetType() == OBJ_TYPE::EFFECT) {
			ExplosionEffect* b = (ExplosionEffect*)(itr);
			if (b->IsFinished() == true) {
				delete b;
				itr = nullptr;//ポインタをnullptrにしておく
			}
		}
	}
	//次に、箱の中身を確認して、nullptrがあったら箱から消す
	for (auto it = objects.begin(); it != objects.end();) {
		if (*it == nullptr) {
			it = objects.erase(it);
		}
		else {
			it++;
		}
	}
}

void Stage::shootBullet()
{
	Player* player = nullptr;
	for (auto& obj : objects) {
		if (obj->GetType() == OBJ_TYPE::PLAYER) {
			player = (Player*)obj;
		}
	}

	Vector2D pos = player->GetPos();
	Vector2D v = Math2D::Mul(player->GetDirVec(), 300.0f);
	unsigned int bcol = GetColor(255, 255, 255);
	float r = 2;
	float life = 2.0f;

	Bullet* b = new Bullet(pos, v, bcol, r, life);
	::AddObject(b);
}

void Stage::TitleUpdate()
{
	if (Input::IsKeyDown(KEY_INPUT_NUMPADENTER)) {
		stageState = 1;
	}
}

void Stage::PlayUpdate()
{
	//当たり判定
	Player_vs_Enemy();
	if (stageState == 2) return;
	Enemy_vs_Bullet();

	//消す処理
	DeleteBullet();
	DeleteEnemy();
	DeleteEffect();

	//すべてのオブジェクトを更新
	UpdateAllObjects();

	//クリア判定
	bool isAnyEnemyAlive = false;
	for (const auto& obj : objects) {
		if (obj->GetType() == OBJ_TYPE::ENEMY) {
			isAnyEnemyAlive = true;
			break;
		}
	}
	if (!isAnyEnemyAlive) {
		stageState = 3;
		clearTimer_ = 0;
	}

	//Zキーが押されたら弾丸を生成
	if (Input::IsKeyDown(KEY_INPUT_W)) {
		shootBullet();
	}
}

void Stage::GameOverUpdate()
{
	UpdateAllObjects();
	DeleteEffect();
	if (Input::IsKeyDown(KEY_INPUT_SPACE)) {
		Initialize();
	}
}

void Stage::ClearUpdate()
{
	clearTimer_++;
	if ((int)clearTimer_ % 15 == 0) {//15フレームごとにランダム
		float fx = (float)(GetRand(WIN_WIDTH - 200) + 100);
		float fy = (float)(GetRand(WIN_HEIGHT - 200) + 100);
		ExplosionEffect* firework = new ExplosionEffect({ fx, fy }, 60 + GetRand(40));
		firework->SetCharaColor(GetColor(GetRand(255), GetRand(255), GetRand(255)));
		::AddObject(firework);
	}

	UpdateAllObjects();
	DeleteEffect();
	if (Input::IsKeyDown(KEY_INPUT_SPACE)) {
		Initialize();
	}
}

void Stage::TitleDraw()
{
	//タイトル画面の描画処理
	int fsize = GetFontSize();
	SetFontSize(80);
	SetFontThickness(10);
	DrawString(WIN_WIDTH / 3.2 + 4, WIN_HEIGHT / 3 + 4, "ASTEROIDS", GetColor(255, 0, 0), gameScore_);//影
	DrawString(WIN_WIDTH / 3.2, WIN_HEIGHT / 3, "ASTEROIDS", GetColor(255, 255, 255), gameScore_);//手前
	SetFontSize(fsize);

	SetFontSize(fsize * 2);
	DrawString(WIN_WIDTH / 3, WIN_HEIGHT / 2, "Enterキーでプレイ", GetColor(255, 255, 255));
	SetFontSize(fsize);
}

void Stage::PlayDraw()
{
	DrawAllObjects();
	int fsize = GetFontSize();
	SetFontSize(fsize * 2);
	DrawFormatString(10, 10, GetColor(255, 255, 255), "SCORE:%llu", gameScore_);
	SetFontSize(fsize);
}

void Stage::GameOverDraw()
{
	DrawAllObjects();

	int fsize = GetFontSize();
	SetFontSize(100);
	SetFontThickness(10);
	DrawString(WIN_WIDTH / 3.5 + 4, WIN_HEIGHT / 4 + 4, "GAMEOVER", GetColor(255, 0, 0), gameScore_);//影
	DrawString(WIN_WIDTH / 3.5, WIN_HEIGHT / 4, "GAMEOVER", GetColor(255, 255, 255), gameScore_);//手前
	
	char scoreText[64];
	SetFontSize(80);
	SetFontThickness(10);
	DrawFormatString(291, 350, GetColor(255, 255, 255), "SCORE:%llu", gameScore_);

	SetFontSize(fsize * 2);
	DrawString(WIN_WIDTH / 3, WIN_HEIGHT / 1.65, "SPACEキーでタイトル", GetColor(255, 255, 255));
	SetFontSize(fsize);
}

void Stage::ClearDraw()
{
	DrawAllObjects();

	int fsize = GetFontSize();
	SetFontSize(100);
	SetFontThickness(10);
	DrawString(WIN_WIDTH / 3 + 4, WIN_HEIGHT / 4 + 4, "CLEAR!", GetColor(10, 10, 40), gameScore_);//影
	DrawString(WIN_WIDTH / 3, WIN_HEIGHT / 4, "CLEAR!", GetColor(255, 215, 0), gameScore_);//手前
	
	char scoreText[64];
	SetFontSize(80);
	SetFontThickness(10);
	DrawFormatString(291, 350, GetColor(255, 255, 255), "SCORE:%llu", gameScore_);
	
	SetFontSize(fsize * 2);
	DrawString(WIN_WIDTH / 3, WIN_HEIGHT / 1.65, "SPACEキーでタイトル", GetColor(200, 200, 200));
	SetFontSize(fsize);
}

void Stage::Enemy_vs_Bullet()
{
	//敵vs弾の当たり判定
	//敵の位置と、当たり判定の半径
	//弾の位置
	//isAlive_->falseにする手段
	std::vector<Enemy*> aliveEnemies;
	std::vector<Bullet*> aliveBullet;

	aliveEnemies.clear();//念のため空にする
	aliveBullet.clear();//念のため空にする

	//for (int i = 0; i < objects.size(); i++){}とも書ける
	for (auto& obj : objects) {
		if (obj->GetType() == OBJ_TYPE::ENEMY) {
			//baseクラスのポインタを敵クラスのポインタに変換してる
			Enemy* e = (Enemy*)obj;
			if (e->IsAlive()) {
				aliveEnemies.push_back(e);
			}
		}
		else if (obj->GetType() == OBJ_TYPE::BULLET) {
			//baseクラスのポインタを敵クラスのポインタに変換してる
			Bullet* b = (Bullet*)obj;
			if (!b->IsDead()) {
				aliveBullet.push_back(b);
			}
		}
	}

	for (auto& bullet : aliveBullet) {
		for (auto& enemy : aliveEnemies) {
			float dist = Math2D::Length(Math2D::Sub(bullet->GetPos(), enemy->GetPos()));
			if (dist < enemy->GetCollisionRadius()) {
				//当たった
				enemy->Dead();//敵を消す
				int sc[3] = { 20, 50, 100 };//大中小のスコア
				gameScore_ += sc[enemy->GetSize()];//スコア加算
				//TODO
				//分裂の処理をここでやりたい
				//大か中か小かを判定して
				//大なら中を2～4つ、中なら小を2～4つ、小なら消してエフェクト生成
				if (enemy->GetSize() != Enemy::Size::SMALL) {
					int num = GetRand(3) + 2;//2～4のランダムな数
					//大きさによって、分裂数変えると素敵
					for (int i = 0; i < num; i++) {
						Enemy* e = nullptr;
						if (enemy->GetSize() == Enemy::Size::LARGE) {
							e = new Enemy(Enemy::Size::MEDIUM, 8);
						}
						else {
							e = new Enemy(Enemy::Size::SMALL, 8);
						}
						e->SetPos(enemy->GetPos());
						//速さの設定は必要
						e->SetVel({ (float)(GetRand(200) - 100), (float)(GetRand(200) - 100) });
						::AddObject(e);
					}
				}
				else {
					ExplosionEffect* effect = new ExplosionEffect(enemy->GetPos());
					effect->SetCharaColor(GetColor(GetRand(255), GetRand(255), GetRand(255)));
					::AddObject(effect);
				}
				bullet->Dead();//弾も消す
			}
		}
	}
}

void Stage::Player_vs_Enemy()
{
	std::vector<Enemy*> aliveEnemies;
	aliveEnemies.clear();//念のため空にする
	Player* player = nullptr;

	for (auto& obj : objects) {
		if (obj->GetType() == OBJ_TYPE::PLAYER) {
			player = (Player*)obj;
		}
		else if (obj->GetType() == OBJ_TYPE::ENEMY) {
			//baseクラスのポインタを敵クラスのポインタに変換してる
			Enemy* e = (Enemy*)obj;
			if (e->IsAlive()) {
				aliveEnemies.push_back(e);
			}
		}
	}

	if (player == nullptr || player->IsAlive() == false)
		return;//プレイヤーがいるかどうか

	for (auto& enemy : aliveEnemies) {
		//①敵とプレイヤーの距離を計算
		float dist = Math2D::Length(Math2D::Sub(player->GetPos(), enemy->GetPos()));
		//②敵とプレイヤーの当たり判定の半径を足したものより、
		float collisionDist = player->GetCollisionRadius() + enemy->GetCollisionRadius();
		//　距離が近かったら当たったとする
		if (dist < collisionDist) {
			//プレイヤーを死なせる
			player->Dead();
			//赤いエフェクトを生成
			ExplosionEffect* effect = new ExplosionEffect(enemy->GetPos(), 50);
			effect->SetCharaColor(GetColor(255, 0, 0));
			::AddObject(effect);

			stageState = 2;
			gameoverTimer_ = 0;

			break;
		}
	}
}