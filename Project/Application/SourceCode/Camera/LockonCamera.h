#pragma once
#include "ICamera.h"

// ロックオンカメラのクラス
class LockonCamera : public ICamera
{
private:
	Vec3 mCurCamePos;
	Vec3 mTargetCamePos;

public:
	LockonCamera();
	void Init(Player* player) override;
	void Update() override;

};
