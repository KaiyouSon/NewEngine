#pragma once
#include "NewEngine.h"
#include "MotionManager.h"

class HumanoidBody;
class PlayerBody;

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
	bool mIsPlay;
	bool mIsInit;
	bool mIsCalcCollider;
	bool mIsCanChangeMotion;
	uint32_t mComboCount;
	uint32_t mComboMaxCount;
	uint32_t mStep;
	Easing mEase;
	std::vector<Vec3> mCurRots;
	std::vector<Vec3> mEndRots;
	std::vector<Vec3> mCurWeaponRots;
	std::vector<Vec3> mEndWeaponRots;
	std::vector<Motion*> mMotions;
	AttackType mAttackType;

protected:
	void BaseInit(HumanoidBody* human, const uint32_t index);
	void BasePrevUpdate(PlayerBody* human);
	void BasePostUpdate(const uint32_t index);

public:
	virtual ~IWeaponMotion() {}
	virtual void Init(HumanoidBody* human) = 0;
	virtual void WeakMotion(PlayerBody* human) = 0;
	virtual void HeavyMotion(PlayerBody* human) = 0;
	virtual void BackMotion(PlayerBody* human) = 0;
	virtual void RollMotion(PlayerBody* human) = 0;

public:
	void IncreComboCount();
	void ResetComboCount();

public:
	void SetisPlay(const bool isPlay);

public:
	bool GetisPlay();
	bool GetisCalcCollider();
	bool GetisCanChangeMotion();
	AttackType GetAttackType();
};

