#pragma once
#include "NewEngine.h"

class HumanoidBody;

class IWeaponMotion
{
protected:
	bool isPlay_;
	bool isInit_;
	bool isCalcCollider_;
	bool isCanChangeMotion_;
	uint32_t comboCount_;
	uint32_t comboMaxCount_;
	uint32_t step_;
	Easing ease_;
	std::vector<Vec3> startRots_;
	std::vector<Vec3> endRots_;

public:
	virtual ~IWeaponMotion() {}
	virtual void Init() = 0;
	virtual void AttackMotion(HumanoidBody* human) = 0;

public:
	void IncreComboCount();
	void ResetComboCount();

public:
	void SetisPlay(const bool isPlay);

public:
	bool GetisPlay();
	bool GetisCalcCollider();
	bool GetisCanChangeMotion();
};
