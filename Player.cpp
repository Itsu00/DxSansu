#include "Player.h"
#include <DxLib.h>
#include <cmath>
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
	//三角形の３頂点をdir_(ベクトル)とradius_(半径)とpos_(中心)から求める
	vertex_[0] = { pos_.x + 0, pos_.y + radius_ };
	vertex_[1] = { pos_.x + radius_ * cos(-60.0f * (PI / 180.0f)),
				   pos_.y + radius_ * sin(-60.0f * (PI / 180.0f)) };
	vertex_[2] = { pos_.x + radius_ * cos(240.0f * (PI / 180.0f)),
				   pos_.y + radius_ * sin(240.0f * (PI / 180.0f)) };
}

void Player::Draw()
{
	DrawTriangleAA(vertex_[0].x, vertex_[0].y,
				   vertex_[1].x, vertex_[1].y,
				   vertex_[2].x, vertex_[2].y,
				   GetColor(255, 0, 0), TRUE);
}