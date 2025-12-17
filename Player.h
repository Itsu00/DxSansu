#pragma once
#include "Base.h"
class Player :
    public Base
{
public:
    Player();//コンストラクタ
	Player(const Vector2D& pos, const Vector2D& vel, unsigned int color,
		   const Vector2D& dir, float r, float omega);//コンストラクタ
    ~Player();//デストラクタ
    void Update() override;//オーバーライド
    void Draw() override;//オーバーライド
public://セッターゲッターは全部インラインで書いたよ
    void SetDirVec(const Vector2D& dir) { dir_ = dir; }
    Vector2D GetDirVec() { return dir_; }
    void SetRadius(float r) { radius_ = r; }
    float GetRadius() { return radius_; }
    void SetOmega(float omega) { omega_ = omega; }
    float GetOmega() { return omega_; }
    void SetAngle(float angle) { angle_ = angle; }
    float GetAngle() { return angle_; }
private:
    Vector2D dir_;//自分の向き（ベクトル）
    float radius_;//半径
    Vector2D vertex_[3];//三角形の三点の座標
    float omega_;//回転速度(角速度)
    float angle_;//現在の回転角
};