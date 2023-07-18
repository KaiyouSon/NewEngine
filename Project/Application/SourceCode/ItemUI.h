#pragma once
#include "NewEngine.h"

enum class ItemType
{
	None,

	// ìπãÔ
	HPBottle,
	MPBottle,

	// ïêäÌ
	Club,
};

class ItemUI
{
private:
	std::unique_ptr<Sprite> itemUI_;
	ItemType itemType_;

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

