#pragma once
#include "ICamera.h"

class TargetCamera : public ICamera
{
private:
	Vec3 mCurCamePos;
	Vec3 mTargetCamePos;

public:
	TargetCamera();
	void Init(Player* player) override;
	void Update() override;

};
