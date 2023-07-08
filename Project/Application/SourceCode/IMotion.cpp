#include "IMotion.h"
#include "HumanoidBody.h"

void IMotion::BaseInit(HumanoidBody* human)
{
	if (isInit_ == false)
	{
		// Œ»Žž“_‚Ìƒ‚[ƒVƒ‡ƒ“‚Ì‰Šú‰»
		curRots_ = human->CalcCurRots();
		MotionData current = motion_->data[step_];
		for (uint32_t i = 0; i < current.endRots.size(); i++)
		{
			endRots_[i] = current.endRots[i];
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
