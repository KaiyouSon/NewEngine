#include "BossAttack2Motion.h"
#include "HumanoidBody.h"
#include "Boss.h"

BossAttack2Motion::BossAttack2Motion()
{
	mMotion = MotionManager::GetMotion("BossAttack2");
}

void BossAttack2Motion::Init(HumanoidBody* human)
{
	mCurRots.resize(human->GetPartsSize());
	mEndRots.resize(human->GetPartsSize());
	mCurWeaponPoses.resize(human->GetWeaponPartsSize());
	mEndWeaponPoses.resize(human->GetWeaponPartsSize());
	mCurWeaponRots.resize(human->GetWeaponPartsSize());
	mEndWeaponRots.resize(human->GetWeaponPartsSize());
}
void BossAttack2Motion::Update(HumanoidBody* human)
{
	if (mIsPlay == false)
	{
		mIsPlay = true;
	}

	mEase.Update();

	if (mIsInit == false)
	{
		BaseInit(human);
		CurrentStepInit(human);
		mIsInit = true;
	}
	BasePrevUpdate(human);
	CurrentStepUpdate(human);
	BasePostUpdate();
}

void BossAttack2Motion::CurrentStepInit(HumanoidBody* human)
{
	Boss* boss = static_cast<Boss*>(human->iParent);
	mStartBodyY = human->GetPart(PartID::Body)->pos.y;
	switch (mStep)
	{
	case 0:
		mEndBodyY = -0.9f;
		break;
	case 1:
		mEndBodyY = -1.1f;
		boss->GetWeapon()->SetisActiveTrajectory(true);
		break;
	case 2:
		SettingMovePrame(human, 5, 30, 2);
		boss->GetWeapon()->SetisCalcCollider(true);
		boss->mDamage = 40.f;
		mEndBodyY = -0.85f;
		break;
	case 3:
		mEndBodyY = -1.0f;
		break;
	case 4:
		mEndBodyY = -1.3f;
		break;
	case 5:
		mEndBodyY = -0.55f;
		boss->GetWeapon()->SetisCalcCollider(false);
		boss->mDamage = 0.f;
		break;
	case 6:
		mEndBodyY = -0.5f;
		break;
	case 7:
	case 8:
		mEndBodyY = -0.6f;
		break;
	case 9:
		mEndBodyY = -0.55f;
		break;
	case 10:
		mEndBodyY = -0.85f;
		break;
	case 11:
		SettingMovePrame(human, 5, 30, 2);
		boss->GetWeapon()->SetisCalcCollider(true);
		boss->mDamage = 40.f;
		mEndBodyY = -1.3f;
		break;
	case 12:
		mEndBodyY = -1.25f;
		break;
	case 13:
		mEndBodyY = -1.3f;
		break;
	case 14:
		mEndBodyY = -0.6f;
		boss->GetWeapon()->SetisCalcCollider(false);
		boss->mDamage = 0.f;
		break;
	case 15:
		mEndBodyY = 0.0f;
		boss->GetWeapon()->SetisCalcCollider(true);
		boss->mDamage = 60.f;
		break;
	case 16:
		SettingMovePrame(human, 8, 20, 2);
		mEndBodyY = -1.75f;
		break;
	case 17:
		boss->GetWeapon()->SetisCalcCollider(false);
		boss->GetWeapon()->SetisActiveTrajectory(false);
		boss->mDamage = 0.f;
		mEndBodyY = -0.7f;
		break;
	default:
		mEndBodyY = 0;
	}
}
void BossAttack2Motion::CurrentStepUpdate(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = mEase.Interpolation(mStartBodyY, mEndBodyY);

	if ((mStep >= 2 && mStep <= 4) ||
		(mStep >= 11 && mStep <= 13) ||
		(mStep == 16))
	{
		human->pos = mMoveEase.InOut(mStartPos, mEndPos);
		mMoveEase.Update();
	}

	if (mStep == 2)
	{
		if (mEase.GetisEnd() == true)
		{
			SoundManager::Play("BossAttackSE");
		}
	}
	else if (mStep == 11)
	{
		if (mEase.GetisEnd() == true)
		{
			SoundManager::Play("BossAttackSE");
		}
	}
	else if (mStep == 16)
	{
		if (mEase.GetTimer() == 10)
		{
			SoundManager::Play("BossAttackSE");
		}
	}
}

void BossAttack2Motion::SettingMovePrame(HumanoidBody* human, const float dis, const uint32_t easeTimer, const float powNum)
{
	Boss* boss = static_cast<Boss*>(human->iParent);

	mMoveEase.SetEaseTimer(easeTimer);
	mMoveEase.SetPowNum(powNum);
	mMoveEase.Reset();
	boss->CalcFrontVec();

	mStartPos = human->pos;
	mEndPos = mStartPos + boss->mFrontVec.Norm() * dis;
}
