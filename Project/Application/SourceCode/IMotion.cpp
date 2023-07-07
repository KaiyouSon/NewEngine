#include "IMotion.h"
#include "HumanoidBody.h"

void IMotion::BaseUpdate(HumanoidBody* human)
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
	for (uint32_t i = (uint32_t)PartID::Body; i < curRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.Interpolation(curRots_[i], endRots_[i]);
	}
	ease_.Update();
}

bool IMotion::GetisPlay()
{
	return isPlay_;
}

bool IMotion::GetisCanChangeMotion()
{
	return isCanChangeMotion_;
}
