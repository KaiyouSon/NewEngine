#include "IWeaponMotion.h"
#include "HumanoidBody.h"

void IWeaponMotion::BaseUpdate(HumanoidBody* human, const AttackType type)
{
	if (isInit_ == false)
	{
		// Œ»Žž“_‚Ìƒ‚[ƒVƒ‡ƒ“‚Ì‰Šú‰»
		curRots_ = human->CalcCurRots();
		MotionData current = motions_[(uint32_t)type]->data[step_];
		for (uint32_t i = 0; i < current.endRots.size(); i++)
		{
			endRots_[i] = current.endRots[i];
		}
		ease_ = current.ease;
		isInit_ = true;
	}
	for (uint32_t i = (uint32_t)PartID::Body; i < curRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.Interpolation(curRots_[i], endRots_[i]);
	}
	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_++;
		if (step_ >= motions_[(uint32_t)type]->data.size())
		{
			step_ = 0;
		}
		isInit_ = false;
		ease_.Reset();
	}
}

void IWeaponMotion::IncreComboCount()
{
	comboCount_++;
}

void IWeaponMotion::ResetComboCount()
{
	comboCount_ = 1;
}

void IWeaponMotion::SetisPlay(const bool isPlay)
{
	isPlay_ = isPlay;
}

bool IWeaponMotion::GetisPlay()
{
	return isPlay_;
}

bool IWeaponMotion::GetisCalcCollider()
{
	return isCalcCollider_;
}

bool IWeaponMotion::GetisCanChangeMotion()
{
	return isCanChangeMotion_;
}