#pragma once
#include "NewEngine.h"

class RespawnPointUI
{
private:
	std::unique_ptr<Sprite> mBack;


public:
	RespawnPointUI();
	void Init();
	void Update();
	void DrawFrontSprite();
};

