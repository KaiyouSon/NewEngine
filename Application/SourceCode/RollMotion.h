#pragma once
#include "IMotion.h"

class RollMotion : public IMotion
{
private:
	void Step0Init(HumanoidBody* human);
	void Step0RotsInit(HumanoidBody* human);
	void Step0Update(HumanoidBody* human);

public:
	void Init(HumanoidBody* human) override;
	void Update(HumanoidBody* human) override;
	
};

