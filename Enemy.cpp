#include "Enemy.h"
#include "globals.h"
#include <DxLib.h>
#include "Math2D.h"

namespace
{
	const int SEGMENT_NUM = 8;//円の分割数
	//位置はコンストラクタでランダムに決定
	//速度もコンストラクタでランダムに決定(範囲あり、方向と大きさ)
	//半径もコンストラクタでランダムに決定(範囲あり)
	const float MIN_RADIUS = 25.0f;//最小半径
}

Enemy::Enemy(int segment)
	:Base(), segment_(segment)
{
	if (segment_ < SEGMENT_NUM) segment_ = SEGMENT_NUM;//最低8分割
	radius_ = (float)(GetRand(50) + MIN_RADIUS);//25から75の間のランダムな半径

	//pos_を画面内のランダムな位置に設定
	//pos_ = { (float)GetRand(WIN_WIDTH - 1), (float)GetRand(WIN_HEIGHT - 1) };
	// 画面外ランダム生成
	int side = GetRand(3);
	switch (side)
	{
	case 0: // 上
		pos_ = { (float)GetRand(WIN_WIDTH), -radius_ };
		break;
	case 1: // 右
		pos_ = { WIN_WIDTH + radius_, (float)GetRand(WIN_HEIGHT) };
		break;
	case 2: // 下
		pos_ = { (float)GetRand(WIN_WIDTH), WIN_HEIGHT + radius_ };
		break;
	case 3: // 左
		pos_ = { -radius_, (float)GetRand(WIN_HEIGHT) };
		break;
	}

	//x,yともに-100から100の間のランダムな速度
	vel_ = { (float)(GetRand(200) - 100), (float)(GetRand(200) - 100) };

	Color_ = GetColor(255, 255, 255);//白
	vertex_.resize(segment_);//頂点配列のサイズ変更 .resize(size) で変更できる
	MaleShape();//頂点座標初期化
}

void Enemy::Update()
{
	pos_ = Math2D::Add(pos_, Math2D::Mul(vel_, 0.02f));

	// 画面端ワープ
	if (pos_.x < -radius_) pos_.x = WIN_WIDTH + radius_;
	if (pos_.x > WIN_WIDTH + radius_) pos_.x = -radius_;

	if (pos_.y < -radius_) pos_.y = WIN_HEIGHT + radius_;
	if (pos_.y > WIN_HEIGHT + radius_) pos_.y = -radius_;

	// 形状の再配置（重要）
	for (int i = 0; i < segment_; i++)
	{
		float angle = (2.0f * DX_PI / segment_) * i;
		vertex_[i] = {
			radius_ * cosf(angle) + pos_.x,
			radius_ * sinf(angle) + pos_.y
		};
	}
}

void Enemy::Draw()
{
	//スクリーン座標に変換した頂点配列を作る
	std::vector<Vector2D> scrVertex(segment_);
	for (int i = 0; i < segment_; i++)
	{
		scrVertex[i] = Math2D::World2Screen(vertex_[i]);
	}
	/*Vector2D Start = scrVertex[0];
	for (int i = 1; i < segment_; i++)
	{
		Vector2D End = scrVertex[i];
		DrawLineAA(Start.x, Start.y, 
				   End.x, End.y, 
				   GetColor(255, 255, 255), 1.0f);
		Start = End;
	}*/

	for (int i = 0; i < segment_; i++)
	{
		int next = (i + 1) % segment_;

		DrawLineAA(
			scrVertex[i].x, scrVertex[i].y,
			scrVertex[next].x, scrVertex[next].y,
			GetColor(255, 255, 255),
			1.0f
		);
	}
}

void Enemy::MaleShape()
{
	for (int i = 0; i < segment_; i++)
	{
		float angle = (2.0f * DX_PI / segment_) * i;
		vertex_[i] = { radius_ * cosf(angle), radius_ * sinf(angle) };
		vertex_[i] = Math2D::Add(vertex_[i], pos_);
	}
}