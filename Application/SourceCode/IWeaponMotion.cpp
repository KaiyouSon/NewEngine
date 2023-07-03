#include "IWeaponMotion.h"

void IWeaponMotion::IncreComboCount()
{
	comboCount_++;
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

bool IWeaponMotion::GetisCanCombo()
{
	return isCanCombo_;
}
