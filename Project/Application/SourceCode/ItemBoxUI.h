#pragma once
#include "NewEngine.h"

class ItemBoxUI
{
private:
	std::unique_ptr<Sprite> frame_;

public:
	ItemBoxUI();
	void Init();
	void Update();
	void DrawFrontSprite();

};

