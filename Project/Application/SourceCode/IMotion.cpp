#include "IMotion.h"
#include "HumanoidBody.h"

IMotion::IMotion() :
	isInit_(false), isPlay_(false),
	isCanChangeMotion_(false),
	step_(0), motion_(nullptr)
{
}

void IMotion::BaseInit(HumanoidBody* human)
{
	if (isInit_ == false)
	{
		//human->pos.y = 4.75f;
		//human->GetPart(PartID::Body)->pos.y = 0.f;

		// Œ»Žž“_‚Ìƒ‚[ƒVƒ‡ƒ“‚Ì‰Šú‰»
		curRots_ = human->CalcCurRots();
		MotionData current = motion_->data[step_];
		for (uint32_t i = 0; i < current.endRots.size(); i++)
		{
			endRots_[i] = current.endRots[i];
		}

		curWeaponPoses_ = human->CalcCurWeaponPoses();
		curWeaponRots_ = human->CalcCurWeaponRots();
		for (uint32_t i = 0; i < current.endWeaponPoses.size(); i++)
		{
			endWeaponPoses_[i] = current.endWeaponPoses[i];
			endWeaponRots_[i] = current.endWeaponRots[i];
		}
		ease_ = current.ease;
	}
}

void IMotion::BasePrevUpdate(HumanoidBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Body; i < curRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.Interpolation(curRots_[i], endRots_[i]);
	}
	for (uint32_t i = 0; i < curWeaponRots_.size(); i++)
	{
		if (human->GetWeaponPart((WeaponPartID)i) == nullptr)
		{
			continue;
		}

		human->GetWeaponPart((WeaponPartID)i)->pos = ease_.Interpolation(curWeaponPoses_[i], endWeaponPoses_[i]);
		human->GetWeaponPart((WeaponPartID)i)->rot = ease_.Interpolation(curWeaponRots_[i], endWeaponRots_[i]);
	}
}

void IMotion::BasePostUpdate(HumanoidBody* human)
{
	if (ease_.GetisEnd() == true)
	{
		step_++;
		if (step_ >= motion_->data.size())
		{
			step_ = 0;
			isPlay_ = false;
		}
		isInit_ = false;
		ease_.Reset();
	}
}

bool IMotion::GetisPlay()
{
	return isPlay_;
}

bool IMotion::GetisCanChangeMotion()
{
	return isCanChangeMotion_;
}
