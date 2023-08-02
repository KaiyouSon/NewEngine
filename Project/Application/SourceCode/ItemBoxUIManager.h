#pragma once
#include "ItemUI.h"
#include "ItemBoxUI.h"
#include "Player.h"

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
	std::unique_ptr<Sprite> num_;

	Timer notActiveTimer_;
	bool isActive_;
	float alpha_;
	float lightAlpha_;
	Player* mPlayer;

public:
	ItemBoxUIManager();
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	void SetAlpha(const float alpha);
	void SetPlayer(Player* player);
};

