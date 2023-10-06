#pragma once
#include "IMotion.h"

class BossGrabAttackMotion : public IMotion
{
private:
	Easing mMoveEase;
	Vec3 mStartPos;
	Vec3 mEndPos;
	float mLength;
	float mStartBodyY;
	float mEndBodyY;

private:
	void CurrentStepInit(HumanoidBody* human);
	void CurrentStepUpdate(HumanoidBody* human);

	void SettingMovePrame(HumanoidBody* human, const float dis, const uint32_t easeTimer, const float powNum);
public:
	BossGrabAttackMotion();
	void Init(HumanoidBody* human) override;
	void Update(HumanoidBody* human) override;
};
