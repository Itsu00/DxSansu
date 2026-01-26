#pragma once
#include "Base.h"
#include <vector>

class Enemy :
    public Base
{
public:
    Enemy(int segment);
    void Update() override;
    void Draw() override;
    bool IsAlive() { return isAlive_; }
private:
    void MakeShape();//’¸“_À•W‰Šú‰»
private:
    int segment_;//‰~‚Ì•ªŠ„”
    float radius_;//”¼Œa
    float omega_;//‰ñ“]‘¬“x(Šp‘¬“x)
    float angle_;//Œ»İ‚Ì‰ñ“]Šp
    bool isAlive_;//¶‘¶ƒtƒ‰ƒO true:¶‚«‚Ä‚¢‚é false:€‚ñ‚Å‚é
    std::vector<Vector2D> vertex_;//‰~‚Ì’¸“_À•W
};