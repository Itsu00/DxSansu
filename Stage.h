#pragma once
#include <vector>

//‘O•ûéŒ¾
class Player;
class Enemy;

class Stage
{
public:
	Stage();
	~Stage();
	void Initialize();
	void Update();
	void Draw();
	void Release();
private:
	void DeleteBullet();
	void DeleteEnemy();
	void shootBullet();
};