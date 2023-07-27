#pragma once
#include "NewEngine.h"

class ItemBoxUI
{
private:
	std::unique_ptr<Sprite> frame_;
	std::unique_ptr<Sprite> light_;
	bool isLightActive_;
	Timer lightActiveTimer_;

public:
	ItemBoxUI();
	void Init();
	void Update(Transform* parent);
	void DrawFrame();
	void DrawLight();

public:
	void SetPos(const Vec2 pos);
	void SetScale(const Vec2 scale);
	void SetFrameAlpha(const float alpha);
	void SetisLightActive(const bool isLightActive);
};

