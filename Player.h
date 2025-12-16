#pragma once
#include "Base.h"
class Player :
    public Base
{
public:
    Player();
    ~Player();
    void Update() override;
    void Draw() override;
private:
    Vector2D direction;//自分の向き（ベクトル）
    float angle;// 向きの角度（2D用）
    Vector2D vertices[3];//三角形の三点の座標
};