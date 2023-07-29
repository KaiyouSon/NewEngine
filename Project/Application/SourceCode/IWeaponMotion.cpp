#include "IWeaponMotion.h"
#include "HumanoidBody.h"
#include "PlayerBody.h"


void IWeaponMotion::BaseInit(HumanoidBody* human, const uint32_t index)
{
	if (isInit_ == false)
	{
		human->pos.y = 4.75f;
		human->GetPart(PartID::Body)->pos.y = 0.f;

		// 現時点のモーションの初期化
		curRots_ = human->CalcCurRots();
		MotionData current = motions_[(uint32_t)index]->data[step_];
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

		isInit_ = true;
	}
}

void IWeaponMotion::BaseUpdate(PlayerBody* human, const uint32_t index)
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

	if (ease_.GetisEnd() == true)
	{
		step_++;
		//if (step_ == 1)
		//{
		//	step_ = 10;
		//}

		if (step_ >= motions_[(uint32_t)index]->data.size())
		{
			step_ = 0;
			isPlay_ = false;
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

AttackType IWeaponMotion::GetAttackType()
{
	return attackType_;
}
