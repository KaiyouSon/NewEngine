#pragma once
#include "GaugeUI.h"
#include "IUI.h"

// プレイヤーのゲージUIクラス
class PlayerGaugeUI : public IUI
{
private:
	std::array<std::unique_ptr<GaugeUI>, 3> mGauges;
	std::array<GaugeParam, 3> mGaugeParam;

public:
	PlayerGaugeUI();
	void Init() override;
	void Update() override;
	void Draw() override;

public:
	void SetGaugeParam(const GaugeParam gaugeParam, const uint32_t index);
};

