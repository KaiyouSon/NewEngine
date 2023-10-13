#pragma once
#include "NewEngine.h"
#include "GaugeParam.h"
#include "GaugeType.h"

// ゲージUIのクラス
class GaugeUI
{
public:
	enum
	{
		// 可変
		CurrentPos,
		FrontColor,
		BackColor,

		// 不変
		BackFrame,
		FrontFrame,
		FrontLeftFrame,
		FrontRightFrame,
	};

private:
	std::array<std::unique_ptr<Sprite>, 7> mSprites;
	Vec2 mBasePos;
	Vec2 mGaugeSize;
	Vec2 mBackGaugeSize;
	Vec2 mConstGaugeSize;
	float mGaugeWidthMax;

	Timer mStayTimer;

private:
	void CalcPosUpdate();
	void CalcSizeUpdate();

public:
	GaugeUI();
	void Init();
	void Update();
	void Draw();

public:
	void SetPos(const Vec2 pos);
	void SetColor(const uint32_t index, const Color color);
	void SetAlpha(const float alpha);
	void SetGaugePrame(const GaugeParam gaugeParam);
};
