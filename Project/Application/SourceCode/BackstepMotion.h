#pragma once
#include "NewEngine.h"
#include "IMotion.h"
#include "MotionManager.h"

class BackstepMotion : public IMotion
{
private:
	Easing moveEase_;
	Vec3 startPos_;
	Vec3 endPos_;
	float length_;
	float up_;
	float down_;


private:
	void CurrentStepInit(PlayerBody* human);
	void CurrentStepUpdate(PlayerBody* human);

	void Step0Init(PlayerBody* human);
	void Step0Update(PlayerBody* human);
	void Step1Update(PlayerBody* human);
	void Step2Update(PlayerBody* human);

public:
	BackstepMotion();
	void Init(PlayerBody* human) override;
	void Update(PlayerBody* human) override;
};

