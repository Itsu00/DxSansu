#pragma once
#include <vector>

//ëOï˚êÈåæ
class Player;
class Enemy;

class Stage
{
public:
	Stage();
	~Stage();
	void Initialize();

	void TitleUpdate();
	void PlayUpdate();
	void GameOverUpdate();
	void ClearUpdate();

	void TitleDraw();
	void PlayDraw();
	void GameOverDraw();
	void ClearDraw();

	//ëSëÃÇÃUpdateÅADraw
	void Update();
	void Draw();
	void Release();
private:
	unsigned long long gameScore_;
	void Enemy_vs_Bullet();
	void Player_vs_Enemy();
	void DeleteBullet();
	void DeleteEnemy();
	void DeleteEffect();
	void shootBullet();
	int gameoverTimer_;
};