#pragma once
#include "IMotion.h"

// ドアを開けるモーションのクラス
class OpenGateMotion : public IMotion
{
private:
	Vec3 mStartPos;
	Vec3 mEndPos;

	Easing mMoveEase;

private:
	void CurrentStepInit(HumanoidBody* human);
	void CurrentStepUpdate(HumanoidBody* human);

public:
	OpenGateMotion();
	void Init(HumanoidBody* human) override;
	void Update(HumanoidBody* human) override;
};
