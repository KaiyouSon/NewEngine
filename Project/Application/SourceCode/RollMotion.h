#pragma once
#include "IMotion.h"

class RollMotion : public IMotion
{
private:
	Easing moveEase_;
	Vec3 startPos_;
	Vec3 endPos_;
	float length_;
	float rotY_;

private:
	void CurrentStepInit(PlayerBody* human);
	void CurrentStepUpdate(PlayerBody* human);

	void Step0Init(PlayerBody* human);
	void Step0Update(PlayerBody* human);
	void Step1Update(PlayerBody* human);
	void Step2Update(PlayerBody* human);
	void Step3Update(PlayerBody* human);

public:
	RollMotion();
	void Init(PlayerBody* human) override;
	void Update(PlayerBody* human) override;

};

