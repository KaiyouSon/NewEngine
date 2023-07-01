#pragma once
#include "NewEngine.h"
#include "GaugeParam.h"
#include "HumanoidBody.h"

class Player
{
private:
	std::array<GaugeParam, 3> gaugePrames_;

	std::unique_ptr<HumanoidBody> player_;


private:
	void GaugeParamInit();

public:
	Player();
	void Init();
	void Update();
	void DrawModel();
	void DrawDebugGui();

public:
	GaugeParam GetGaugeParam(const uint32_t index);
};

