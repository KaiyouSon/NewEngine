#pragma once
#include "IMotion.h"

class RollMotion : public IMotion
{
private:
	Easing moveEase_;
	Vec3 startPos_;
	Vec3 endPos_;
	float length_;

private:
	void CurrentStepInit(HumanoidBody* human);
	void CurrentStepUpdate(HumanoidBody* human);

	void Step0Init(HumanoidBody* human);
	void Step0Update(HumanoidBody* human);
	void Step1Update(HumanoidBody* human);
	void Step2Update(HumanoidBody* human);
	void Step3Update(HumanoidBody* human);

public:
	RollMotion();
	void Init(HumanoidBody* human) override;
	void Update(HumanoidBody* human) override;

};

