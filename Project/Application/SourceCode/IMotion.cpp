#include "IMotion.h"
#include "HumanoidBody.h"

IMotion::IMotion() :
	mIsInit(false), mIsPlay(false),
	mIsCanChangeMotion(false),
	mStep(0), mMotion(nullptr)
{
}

void IMotion::BaseInit(HumanoidBody* human)
{
	if (mIsInit == false)
	{
		//human->pos.y = 4.75f;
		//human->GetPart(PartID::Body)->pos.y = 0.f;

		// Œ»Žž“_‚Ìƒ‚[ƒVƒ‡ƒ“‚Ì‰Šú‰»
		mCurRots = human->CalcCurRots();
		MotionData current = mMotion->data[mStep];
		for (uint32_t i = 0; i < current.endRots.size(); i++)
		{
			mEndRots[i] = current.endRots[i];
		}

		mCurWeaponPoses = human->CalcCurWeaponPoses();
		mCurWeaponRots = human->CalcCurWeaponRots();
		for (uint32_t i = 0; i < current.endWeaponPoses.size(); i++)
		{
			mEndWeaponPoses[i] = current.endWeaponPoses[i];
			mEndWeaponRots[i] = current.endWeaponRots[i];
		}
		mEase = current.ease;
	}
}

void IMotion::BasePrevUpdate(HumanoidBody* human)
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

		human->GetWeaponPart((WeaponPartID)i)->pos = mEase.Interpolation(mCurWeaponPoses[i], mEndWeaponPoses[i]);
		human->GetWeaponPart((WeaponPartID)i)->rot = mEase.Interpolation(mCurWeaponRots[i], mEndWeaponRots[i]);
	}
}

void IMotion::BasePostUpdate(HumanoidBody* human)
{
	if (mEase.GetisEnd() == true)
	{
		mStep++;
		if (mStep >= mMotion->data.size())
		{
			mStep = 0;
			mIsPlay = false;
		}
		mIsInit = false;
		mEase.Reset();
	}
}

bool IMotion::GetisPlay()
{
	return mIsPlay;
}

bool IMotion::GetisCanChangeMotion()
{
	return mIsCanChangeMotion;
}
