#pragma once
#include "IMotion.h"

class DrinkMotion : public IMotion
{
private:
	//void CurrentStepInit(HumanoidBody* human);
	//void CurrentStepUpdate(HumanoidBody* human);

	//void Step0Init(HumanoidBody* human);
	//void Step0Update(HumanoidBody* human);
	//void Step1Update(HumanoidBody* human);
	//void Step2Update(HumanoidBody* human);
	//void Step3Update(HumanoidBody* human);

public:
	DrinkMotion();
	void Init(HumanoidBody* human) override;
	void Update(HumanoidBody* human) override;
};

