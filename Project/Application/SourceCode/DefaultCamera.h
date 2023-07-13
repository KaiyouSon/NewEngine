#pragma once
#include "NewEngine.h"
#include "ICamera.h"

class DefaultCamera : public ICamera
{
private:
	float targetYaw_;
	float assistYaw_;
	float controlYaw_;
	float controlPitch_;

	Vec3 vec_;
	Vec3 rot_;

public:
	DefaultCamera();
	void Init(Player* player) override;
	void Update() override;

};

