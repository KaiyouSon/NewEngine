#pragma once
#include "NewEngine.h"
#include "MotionManager.h"

class HumanoidBody;

struct IMotion
{
public:
	bool mIsInit;
	bool mIsPlay;
	bool mIsCanChangeMotion;

	uint32_t mStep;
	Easing mEase;

	std::vector<Vec3> mCurRots;
	std::vector<Vec3> mEndRots;

	std::vector<Vec3> mCurWeaponPoses;
	std::vector<Vec3> mEndWeaponPoses;

	std::vector<Vec3> mCurWeaponRots;
	std::vector<Vec3> mEndWeaponRots;
	Motion* mMotion;

public:
	void BaseInit(HumanoidBody* human);
	void BasePrevUpdate(HumanoidBody* human);
	void BasePostUpdate();

public:
	IMotion();
	virtual void Init(HumanoidBody* human) = 0;
	virtual void Update(HumanoidBody* human) = 0;

public:
	bool GetisPlay();
	bool GetisCanChangeMotion();
};
