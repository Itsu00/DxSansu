#include "Bullet.h"
#include "globals.h"
#include <DxLib.h>

Bullet::Bullet(const Vector2D& pos, const Vector2D& vel, unsigned int color, float radius, float life)
	:Base(pos, vel, color), radius_(radius), life_(life)
{
	SetObjType(BULLET);
}

void Bullet::Update()
{
	if (IsDead() == true) return;//死んだらスルー
	float dt = GetDeltaTime();
	//pos_.x = pos_.x + vel_.x * dt;
	//pos_.y = pos_.y + vel_.y * dt;
	pos_ = Math2D::Add(pos_, Math2D::Mul(vel_, dt));

	//画面端ワープ
	if (pos_.x < 0) pos_.x = WIN_WIDTH;//左端にいったら右端から出てくる
	if (pos_.x > WIN_WIDTH) pos_.x = 0;//右端にいったら左端から出てくる
	if (pos_.y < 0) pos_.y = WIN_HEIGHT;//上を過ぎたら下から出てくる
	if (pos_.y > WIN_HEIGHT) pos_.y = 0;//下を過ぎたら上から出てくる
	//弾が何秒生きるか
	life_ = life_ - dt;
}

void Bullet::Draw()
{
	if (IsDead() == true) return;
	Vector2D sPos = Math2D::World2Screen(pos_);
	DrawCircle(sPos.x, sPos.y, radius_, Color_, TRUE);
}