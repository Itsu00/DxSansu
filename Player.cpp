#include "Player.h"
#include <DxLib.h>
#include <cmath>
#include "globals.h"
#include "Math2D.h"
#include "Input.h"

namespace 
{
	const float DAMP = 0.995f;//減衰率
	const float ACC = 200.0f;//加速度
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
	//初期座標がここまでで決定

	if (Input::IsKeepKeyDown(KEY_INPUT_LEFT))
	{
		angle_ = angle_ + omega_ * GetDeltaTime();
	}
	if (Input::IsKeepKeyDown(KEY_INPUT_RIGHT))
	{
		angle_ = angle_ - omega_ * GetDeltaTime();
	}

	//原点に三角形を戻す
	Mat2 toOrigin = Math2D::Translation({ -pos_.x, -pos_.y });
	/*for (int i = 0; i < 3; i++)
	{
		vertex_[i] = Math2D::TransformPoint(vertex_[i], toOrigin);
	}*/
	Mat2 rotMat = Math2D::Rotation(angle_);//単位はラジアン
	/*for (int i = 0; i < 3; i++)
	{
		vertex_[i] = Math2D::TransformPoint(vertex_[i], rotMat);
	}*/
	Mat2 toPos = Math2D::Translation({ pos_.x, pos_.y });
	/*for (int i = 0; i < 3; i++)
	{
		vertex_[i] = Math2D::TransformPoint(vertex_[i], toPos);
	}*/
	//回転処理完了
	//[toPos*rotMat*toOrigin]*vertex_;
	Mat2 tmp = Math2D::Multiply(rotMat, toOrigin);
	//[toPos*tmp]*vertex_;
	Mat2 M = Math2D::Multiply(toPos, tmp);
	for (int i = 0; i < 3; i++)
	{
		vertex_[i] = Math2D::TransformPoint(vertex_[i], M);
	}

	dir_ = Math2D::FromAngle(angle_ + PI / 2.0f);

	if (Input::IsKeepKeyDown(KEY_INPUT_SPACE))
	{
		vel_.x = vel_.x + dir_.x * ACC * GetDeltaTime();
		vel_.y = vel_.y + dir_.y * ACC * GetDeltaTime();
	}

	//移動処理
	pos_.x = pos_.x + vel_.x * GetDeltaTime();
	pos_.y = pos_.y + vel_.y * GetDeltaTime();
	//pos_ = Math2D::Add(pos_, Math2D::Mul(vel_, GetDeltaTime()));

	vel_ = Math2D::Mul(vel_, DAMP);//減衰処理

	//画面端ワープ
	if (pos_.x < 0) pos_.x = WIN_WIDTH;//左端にいったら右端から出てくる
	if (pos_.x > WIN_WIDTH) pos_.x = 0;//右端にいったら左端から出てくる
	if (pos_.y < 0) pos_.y = WIN_HEIGHT;//上を過ぎたら下から出てくる
	if (pos_.y > WIN_HEIGHT) pos_.y = 0;//下　　　　　　　を過ぎたら上から出てくる
}

void Player::Draw()
{
	Vector2D scrPos[3];
	scrPos[0] = Math2D::World2Screen(vertex_[0]);
	scrPos[1] = Math2D::World2Screen(vertex_[1]);
	scrPos[2] = Math2D::World2Screen(vertex_[2]);

	DrawTriangleAA(scrPos[0].x, scrPos[0].y,
				   scrPos[1].x, scrPos[1].y,
				   scrPos[2].x, scrPos[2].y,
				   GetColor(255, 0, 0), TRUE);

	DrawFormatString(50, 50, GetColor(255, 255, 255), "RotAngle：%lf", angle_);
}