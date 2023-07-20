#pragma once
#include "IMotion.h"

class BossAttack2Motion : public IMotion
{
private:
	Easing moveEase_;
	Vec3 startPos_;
	Vec3 endPos_;
	float length_;
	float startBodyY_;
	float endBodyY_;

private:
	void CurrentStepInit(HumanoidBody* human);
	void CurrentStepUpdate(HumanoidBody* human);

	void SettingMovePrame(HumanoidBody* human, const float dis, const uint32_t easeTimer, const float powNum);
public:
	BossAttack2Motion();
	void Init(HumanoidBody* human) override;
	void Update(HumanoidBody* human) override;
};

