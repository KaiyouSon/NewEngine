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
	Transform mParent;

	std::array<std::unique_ptr<ItemBoxUI>, 4> mItemBoxUIs;
	std::array<std::unique_ptr<ItemUI>, 4> mItemUIs;
	std::unique_ptr<Sprite> mNumber;

	Timer mNotActiveTimer;
	bool mIsActive;
	float mAlpha;
	float mLightAlpha;
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

