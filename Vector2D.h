#pragma once

//カプセル化しない方が楽なので、Vector2Dはカプセル化しない
class Vector2D
{
public:
	float x, y;//メンバ変数
	Vector2D(float _x = 0, float _y = 0);
	Vector2D Add(const Vector2D& _v); // this = this + _v;
	Vector2D ScalarMul(float k); // this = k * this;
	float Distance(const Vector2D& _v); //thisと_vの距離
};