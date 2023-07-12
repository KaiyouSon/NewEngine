#pragma once
#include "ICamera.h"

class TargetCamera : public ICamera
{
private:
	Vec3 curCamePos_;
	Vec3 targetCamePos_;

public:
	TargetCamera();
	void Init(Player* player) override;
	void Update() override;

};

