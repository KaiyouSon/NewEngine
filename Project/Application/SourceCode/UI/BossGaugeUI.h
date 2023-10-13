#pragma once
#include "GaugeUI.h"
#include "IUI.h"

// ボスのゲージUIクラス
class BossGaugeUI : public IUI
{
private:
	std::unique_ptr<GaugeUI> mGauge;
	float mAlpha;

private:
	void AlphaUpdate();

public:
	BossGaugeUI();
	void Init() override;
	void Update() override;
	void Draw() override;

public:
	void SetGaugeParam(const GaugeParam gaugeParam);
};

