#pragma once
#include "NewEngine.h"

class HumanoidBody;

class IWeaponMotion
{
protected:
	bool isPlay_;
	bool isInit_;
	uint32_t step_;
	Easing ease_;
	std::vector<Vec3> curRots_;

public:
	virtual ~IWeaponMotion() {}
	virtual void Init() = 0;
	virtual void AttackMotion(HumanoidBody* human) = 0;

public:
	void SetisPlay(const bool isPlay);

public:
	bool GetisPlay();
};

