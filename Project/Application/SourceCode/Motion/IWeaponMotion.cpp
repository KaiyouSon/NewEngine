#include "IWeaponMotion.h"
#include "HumanoidBody.h"
#include "PlayerBody.h"

void IWeaponMotion::BaseInit(HumanoidBody* human, const uint32_t index)
{
	if (mIsInit == false)
	{
		human->pos.y = 4.75f;

		// Œ»Žž“_‚Ìƒ‚[ƒVƒ‡ƒ“‚Ì‰Šú‰»
		mCurRots = human->CalcCurRots();
		MotionData current = mMotions[(uint32_t)index]->data[mStep];
		for (uint32_t i = 0; i < current.endRots.size(); i++)
		{
			mEndRots[i] = current.endRots[i];
		}
		mCurWeaponRots = human->CalcCurWeaponRots();
		for (uint32_t i = 0; i < current.endWeaponRots.size(); i++)
		{
			mEndWeaponRots[i] = current.endWeaponRots[i];
		}
		mEase = current.ease;
	}
}

void IWeaponMotion::BasePrevUpdate(PlayerBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Body; i < mCurRots.size(); i++)
	{
		human->GetPart((PartID)i)->rot = mEase.Interpolation(mCurRots[i], mEndRots[i]);
	}
	for (uint32_t i = 0; i < mCurWeaponRots.size(); i++)
	{
		if (human->GetWeaponPart((WeaponPartID)i) == nullptr)
		{
			continue;
		}

		human->GetWeaponPart((WeaponPartID)i)->rot = mEase.Interpolation(mCurWeaponRots[i], mEndWeaponRots[i]);
	}
}

void IWeaponMotion::BasePostUpdate(const uint32_t index)
{
	if (mEase.GetisEnd() == true)
	{
		mStep++;

		if (mStep >= mMotions[(uint32_t)index]->data.size())
		{
			mStep = 0;
			mIsPlay = false;
		}
		mIsInit = false;
		mEase.Reset();
	}
}

void IWeaponMotion::IncreComboCount()
{
	mComboCount++;
}

void IWeaponMotion::ResetComboCount()
{
	mComboCount = 1;
}

void IWeaponMotion::SetisPlay(const bool isPlay)
{
	mIsPlay = isPlay;
}

bool IWeaponMotion::GetisPlay()
{
	return mIsPlay;
}

bool IWeaponMotion::GetisCalcCollider()
{
	return mIsCalcCollider;
}

bool IWeaponMotion::GetisCanChangeMotion()
{
	return mIsCanChangeMotion;
}

AttackType IWeaponMotion::GetAttackType()
{
	return mAttackType;
}
