#pragma once
#include "NewEngine.h"
#include "ICamera.h"

class DefaultCamera : public ICamera
{
private:
	float mTargetYaw;
	float mAssistYaw;
	float mControlYaw;
	float mControlPitch;

	Vec3 mVec;
	Vec3 mRot;

public:
	DefaultCamera();
	void Init(Player* player) override;
	void Update() override;

};
