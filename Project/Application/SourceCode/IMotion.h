#pragma once
#include "NewEngine.h"
#include "MotionManager.h"

class PlayerBody;

class IMotion
{
protected:
	bool isInit_;
	bool isPlay_;
	bool isCanChangeMotion_;

	uint32_t step_;
	Easing ease_;

	std::vector<Vec3> curRots_;
	std::vector<Vec3> endRots_;

	std::vector<Vec3> curWeaponRots_;
	std::vector<Vec3> endWeaponRots_;
	Motion* motion_;

protected:
	void BaseInit(PlayerBody* human);
	void BasePrevUpdate(PlayerBody* human);
	void BasePostUpdate(PlayerBody* human);

public:
	virtual void Init(PlayerBody* human) = 0;
	virtual void Update(PlayerBody* human) = 0;

public:
	bool GetisPlay();
	bool GetisCanChangeMotion();
};

