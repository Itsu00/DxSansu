#pragma once
#include <vector>

class Player;
class Stage
{
public:
	Stage();
	~Stage();
	void Initialize();
	void Update();
	void Draw();
	void Release();
private:
	Player* player_;
};