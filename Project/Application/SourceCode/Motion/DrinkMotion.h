#pragma once
#include "IMotion.h"

class DrinkMotion : public IMotion
{
private:
	void CurrentStepUpdate(HumanoidBody* human);

public:
	DrinkMotion();
	void Init(HumanoidBody* human) override;
	void Update(HumanoidBody* human) override;
};
