#pragma once
#include "ItemUI.h"
#include "ItemBoxUI.h"

class ItemBoxUIManager
{
private:
	enum Position
	{
		Left, Right, Up, Down
	};

private:
	Transform parent;

	std::array<std::unique_ptr<ItemBoxUI>, 4> itemBoxUIs_;
	std::array<std::unique_ptr<ItemUI>, 4> itemUIs_;

	float alpha_;

public:
	ItemBoxUIManager();
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	void SetAlpha(const float alpha);
};

