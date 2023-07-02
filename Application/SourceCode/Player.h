#pragma once
#include "NewEngine.h"
#include "GaugeParam.h"
#include "HumanoidBody.h"

class Player
{
private:
	std::array<GaugeParam, 3> gaugePrames_;
	std::unique_ptr<Weapon> weapon_;

	std::unique_ptr<HumanoidBody> player_;
	Vec3 frontVec;

private:
	void GaugeParamInit();
	void MoveUpdate();

public:
	Player();
	void Init();
	void Update();
	void DrawModel();
	void DrawDebugGui();

public:
	GaugeParam GetGaugeParam(const uint32_t index);
	Vec3 GetHeadPos();
};

