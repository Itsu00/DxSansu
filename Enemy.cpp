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
	//pos_を画面内のランダムな位置に設定
	pos_ = { (float)GetRand(WIN_WIDTH - 1), (float)GetRand(WIN_HEIGHT - 1) };
	//x,yともに-100から100の間のランダムな速度
	vel_ = { (float)(GetRand(200) - 100), (float)(GetRand(200) - 100) };
	Color_ = GetColor(255, 255, 255);//白
	if (segment_ < SEGMENT_NUM) segment_ = SEGMENT_NUM;//最低8分割
	radius_ = (float)(GetRand(50) + MIN_RADIUS);//25から75の間のランダムな半径
	vertex_.resize(segment_);//頂点配列のサイズ変更 .resize(size) で変更できる
	MaleShape();//頂点座標初期化
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	//スクリーン座標に変換した頂点配列を作る
	std::vector<Vector2D> scrVertex(segment_);
	for (int i = 0; i < segment_; i++)
	{
		scrVertex[i] = Math2D::World2Screen(vertex_[i]);
	}
	Vector2D Start = scrVertex[0];
	for (int i = 1; i < segment_; i++)
	{
		Vector2D End = scrVertex[i];
		DrawLineAA(Start.x, Start.y, 
				   End.x, End.y, 
				   GetColor(255, 255, 255), 1.0f);
		Start = End;
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