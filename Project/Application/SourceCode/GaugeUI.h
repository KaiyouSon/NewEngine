#pragma once
#include "NewEngine.h"
#include "GaugeParam.h"
#include "GaugeType.h"

class GaugeUI
{
public:
	enum
	{
		// ‰Â•Ï
		CurrentPos,
		FrontColor,
		BackColor,

		// •s•Ï
		BackFrame,
		FrontFrame,
		FrontLeftFrame,
		FrontRightFrame,
	};

private:
	std::array<std::unique_ptr<Sprite>, 7> sprites_;
	Vec2 basePos_;
	Vec2 gaugeSize_;
	Vec2 backGaugeSize_;
	Vec2 constGaugeSize_;
	float gaugeWidthMax_;

	Timer stayTimer;

private:
	void CalcPosUpdate();
	void CalcSizeUpdate();

public:
	GaugeUI();
	void Init();
	void Update();
	void DrawFrontSprite();

public:
	void SetPos(const Vec2 pos);
	void SetColor(const uint32_t index, const Color color);
	void SetGaugePrame(const GaugeParam gaugeParam);
};