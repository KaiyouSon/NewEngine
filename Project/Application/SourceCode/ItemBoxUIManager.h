#pragma once
#include "ItemUI.h"
#include "ItemBoxUI.h"

class ItemBoxUIManager
{
private:
	enum Position
	{
		Left,Right,Up,Down
	};

private:
	Transform parent;

	std::array<std::unique_ptr<ItemBoxUI>, 4> itemBoxUIs_;
	std::unique_ptr<ItemUI> itemUI_;

public:
	ItemBoxUIManager();
	void Init();
	void Update();
	void DrawFrontSprite();
};

