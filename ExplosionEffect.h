#pragma once
#include "Base.h"
#include <vector>

class ExplosionEffect : public Base
{
public:
	ExplosionEffect(const Vector2D& pos, int particleCount = 20);
	void Update() override;
	void Draw() override;
	bool IsFinished() const { return isFinished_; }

	struct Particle
	{
		Vector2D offset;
		Vector2D vel;
		float life;
		float radius;
	};
private:
	bool isFinished_;//エフェクト終了フラグ
	std::vector<Particle> particles_;//パーティクル群
};

