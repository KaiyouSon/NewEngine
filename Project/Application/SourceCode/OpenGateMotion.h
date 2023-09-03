#pragma once
#include "IMotion.h"

class OpenGateMotion : public IMotion
{
private:
	void CurrentStepInit(HumanoidBody* human);
	void CurrentStepUpdate(HumanoidBody* human);

public:
	OpenGateMotion();
	void Init(HumanoidBody* human) override;
	void Update(HumanoidBody* human) override;
};

