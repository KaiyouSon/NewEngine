#pragma once
#include "NewEngine.h"

class ItemBoxUI
{
private:
	std::unique_ptr<Sprite> frame_;

public:
	ItemBoxUI();
	void Init();
	void Update(Transform* parent);
	void DrawFrontSprite();

public:
	void SetPos(const Vec2 pos);
	void SetScale(const Vec2 scale);
};

