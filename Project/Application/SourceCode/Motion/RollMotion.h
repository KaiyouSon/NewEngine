#pragma once
#include "IMotion.h"

// 回転のモーションのクラス
class RollMotion : public IMotion
{
private:
	Easing mMoveEase;
	Vec3 mStartPos;
	Vec3 mEndPos;
	float mLength;
	float mRotY;

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
