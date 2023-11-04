#pragma once
#include "NewEngine.h"
#include "ICamera.h"

// 普段のカメラのクラス
class DefaultCamera : public ICamera
{
private:
	float mControlYaw;
	float mControlPitch;

	Vec3 mVec;
	Vec3 mRot;

public:
	void Init(Player* player) override;
	void Update() override;
};
