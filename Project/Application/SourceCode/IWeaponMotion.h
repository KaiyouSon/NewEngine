#pragma once
#include "NewEngine.h"
#include "MotionManager.h"

class HumanoidBody;

enum class AttackType
{
	Weak,
	Heavy,
	Back,
	Roll,
};

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
	std::vector<Vec3> curRots_;
	std::vector<Vec3> endRots_;
	std::vector<Motion*> motions_;
	AttackType attackType_;

protected:
	void BaseInit(HumanoidBody* human, const uint32_t index);
	void BaseUpdate(HumanoidBody* human, const uint32_t index);

public:
	virtual ~IWeaponMotion() {}
	virtual void Init(HumanoidBody* human) = 0;
	virtual void WeakMotion(HumanoidBody* human) = 0;
	virtual void HeavyMotion(HumanoidBody* human) = 0;
	virtual void BackMotion(HumanoidBody* human) = 0;
	virtual void RollMotion(HumanoidBody* human) = 0;

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

