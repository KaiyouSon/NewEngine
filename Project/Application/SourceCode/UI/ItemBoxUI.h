#pragma once
#include "NewEngine.h"

// アイテムボックスUIのクラス
class ItemBoxUI
{
private:
	std::unique_ptr<Sprite> mFrame;
	std::unique_ptr<Sprite> mLight;
	bool mIsLightActive;
	Timer mLightActiveTimer;

public:
	ItemBoxUI();
	void Init();
	void Update(Transform* parent);
	void DrawFrame();
	void DrawLight();

public:
	// セッター
	void SetPos(const Vec2 pos);
	void SetScale(const Vec2 scale);
	void SetFrameAlpha(const float alpha);
	void SetisLightActive(const bool isLightActive);
};

