#pragma once
#include "GaugeUI.h"
#include "Player.h"

class UIManager
{
private:
	std::array<std::unique_ptr<GaugeUI>, 3> gauges_;
	Player* player_;

public:
	UIManager();
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	void SetPlayer(Player* player);
};

