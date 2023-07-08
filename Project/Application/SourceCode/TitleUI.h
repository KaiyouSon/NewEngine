#pragma once
#include "NewEngine.h"

class TitleUI
{
private:
	std::unique_ptr<Sprite> backColor_;
	std::unique_ptr<Sprite> gameTitle_;
	std::unique_ptr<Sprite> pressButton_;

public:
	TitleUI();
	void Init();
	void Update();
	void DrawFrontSprite();
};

