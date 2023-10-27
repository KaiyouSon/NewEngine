#pragma once
#include "IMotion.h"

// バックステップのモーションのクラス
class BackstepMotion : public IMotion
{
private:
	Easing mMoveEase;
	Vec3 mStartPos;
	Vec3 mEndPos;
	float mLength;
	float mUp;
	float mDown;

private:
	void CurrentStepInit(HumanoidBody* human);
	void CurrentStepUpdate(HumanoidBody* human);

	void Step0Init(HumanoidBody* human);
	void Step0Update(HumanoidBody* human);
	void Step1Update(HumanoidBody* human);
	void Step2Update(HumanoidBody* human);

public:
	BackstepMotion();
	void Init(HumanoidBody* human) override;
	void Update(HumanoidBody* human) override;
};
