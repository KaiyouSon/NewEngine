#include "IMotion.h"
#include "HumanoidBody.h"

void IMotion::BaseInit(HumanoidBody* human)
{
	if (isInit_ == false)
	{
		// 現時点のモーションの初期化
		curRots_ = human->CalcCurRots();
		MotionData current = motion_->data[step_];
		for (uint32_t i = 0; i < current.endRots.size(); i++)
		{
			endRots_[i] = current.endRots[i];
		}
		curWeaponRots_ = human->CalcCurWeaponRots();
		for (uint32_t i = 0; i < current.endWeaponRots.size(); i++)
		{
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

		human->GetWeaponPart((WeaponPartID)i)->rot = ease_.Interpolation(curWeaponRots_[i], endWeaponRots_[i]);
	}
}

void IMotion::BasePostUpdate(HumanoidBody* human)
{
	if (ease_.GetisEnd() == true)
	{
		step_++;
		//if (step_ == 2)
		//{
		//	step_ = 10;
		//}

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
