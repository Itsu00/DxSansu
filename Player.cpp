#include "Player.h"
#include <DxLib.h>
#include <cmath>
#include "globals.h"

namespace {
	Vector2D World2Screen(const Vector2D& wpos)
	{
		Vector2D tmp;//temporary = 作業領域
		tmp.x = wpos.x;
		tmp.y = WIN_HEIGHT - wpos.y;
		return(tmp);
		//return{ wpos.x, WIN_HEIGHT - wpos.y };
	}
}

//引数無しコンストラクタ
//適当な値を入れるだけ、して初期化してないメンバ変数をなくす
//=値に意味はない
Player::Player()
	:Base(Vector2D(0,0), Vector2D(0, 0), GetColor(0, 0, 0)),
	dir_({ 0, -1 }), radius_(1.0f), omega_{1.0f}, angle_(0.0f)
{
	vertex_[0] = { 0, 0 };
	vertex_[1] = { 0, 1 };
	vertex_[2] = { 0, 2 };
}

Player::Player(const Vector2D& pos, const Vector2D& vel, unsigned int color, 
			   const Vector2D& dir, float r, float omega)
	:Base(pos, vel, color), dir_(dir), radius_(r), omega_(omega)
{
	vertex_[0] = { 0, 0 };//0で初期化
	vertex_[1] = { 0, 0 };//0で初期化
	vertex_[2] = { 0, 0 };//0で初期化
	angle_ = 0.0f;//0で初期化
}

Player::~Player()
{
}

void Player::Update()
{
	const float PI = 3.14159265359;
	//原点にある半径1の内接する三角形を考える
	Vector2D p[3];
	p[0] = { 0.0f, 1.0f };
	p[1] = { cos(-60.0f * (PI / 180.0f)), sin(-60.0f * (PI / 180.0f)) };
	p[2] = { cos(240.0f * (PI / 180.0f)), sin(240.0f * (PI / 180.0f)) };
	//半径を1→radius_に拡大
	p[0].x = radius_ * p[0].x; p[0].y = radius_ * p[0].y;
	p[1].x = radius_ * p[1].x; p[1].y = radius_ * p[1].y;
	p[2].x = radius_ * p[2].x; p[2].y = radius_ * p[2].y;
	//三角形の３頂点をdir_(ベクトル)とradius_(半径)とpos_(中心)から求める
	vertex_[0] = { pos_.x + p[0].x, pos_.y + p[0].y };
	vertex_[1] = { pos_.x + p[1].x, pos_.y + p[1].y };
	vertex_[2] = { pos_.x + p[2].x, pos_.y + p[2].y };
}

void Player::Draw()
{
	Vector2D scrPos[3];
	scrPos[0] = World2Screen(vertex_[0]);
	scrPos[1] = World2Screen(vertex_[1]);
	scrPos[2] = World2Screen(vertex_[2]);

	DrawTriangleAA(scrPos[0].x, scrPos[0].y,
				   scrPos[1].x, scrPos[1].y,
				   scrPos[2].x, scrPos[2].y,
				   GetColor(255, 0, 0), TRUE);
}