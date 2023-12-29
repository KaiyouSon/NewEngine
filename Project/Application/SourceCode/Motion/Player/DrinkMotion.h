#pragma once
#include "IMotion.h"

// 回復するときのモーションのクラス
class DrinkMotion : public IMotion
{
private:
	void CurrentStepUpdate(HumanoidBody* human);

public:
	DrinkMotion();
	void Init(HumanoidBody* human) override;
	void Update(HumanoidBody* human) override;
};
