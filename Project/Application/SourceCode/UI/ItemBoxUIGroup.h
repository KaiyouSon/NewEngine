#pragma once
#include "ItemUI.h"
#include "ItemBoxUI.h"
#include "Player.h"
#include "IUI.h"

// アイテムUIを1グループにまとめたクラス
class ItemBoxUIGroup : public IUI
{
private:
	enum Position
	{
		Left, Right, Up, Down, Size
	};

private:
	Transform mParent;
	std::array < std::unique_ptr<ItemBoxUI>, Position::Size> mItemBoxUIs;
	std::array<std::unique_ptr<ItemUI>, Position::Size> mItemUIs;
	std::unique_ptr<Sprite> mNumber;

	Timer mNotActiveTimer;
	bool mIsActive;
	float mAlpha;
	float mLightAlpha;

	uint32_t mBottleNum;

private:
	// スプライトのαの処理
	void AlphaUpdate();

	// ボタンを押したときに光る処理
	void ShainUpdate();

	// アイテムボトルの処理
	void ItemBottleUpdate();

public:
	ItemBoxUIGroup();
	void Init() override;
	void Update() override;
	void Draw() override;

public:
	// セッター
	void SetAlpha(const float alpha);
	void SetBottleNum(const uint32_t bottleNum);
};

