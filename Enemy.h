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
private:
    void MaleShape();//’¸“_À•W‰Šú‰»
private:
    int segment_;//‰~‚Ì•ªŠ„”
    float radius_;//”¼Œa
    std::vector<Vector2D> vertex_;//‰~‚Ì’¸“_À•W
};