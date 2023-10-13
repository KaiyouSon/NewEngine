#pragma once
#include "NewEngine.h"

// アイテムUIの種類
enum class ItemType
{
	None,

	// 道具
	HPBottle,
	MPBottle,
	EmptyBottle,

	// 武器
	Club,
};

// アイテムUIのクラス
class ItemUI
{
private:
	std::unique_ptr<Sprite> mItemUI;
	ItemType mItemType;

private:
	void SettingsByType();

public:
	ItemUI();
	void Init();
	void Update(Transform* parent);
	void DrawFrontSprite();

public:
	void SetPos(const Vec2 pos);
	void SetType(const ItemType itemType);
	void SetAlpha(const float alpha);

};

