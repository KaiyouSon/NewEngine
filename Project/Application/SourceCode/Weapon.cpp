#include "Weapon.h"

float Weapon::GetDamage()
{
	return mDamage;
}

float Weapon::GetChargeRate()
{
	return mChargeRate;
}

void Weapon::SetisCalcCollider(const bool isCalcCollider)
{
	mIsCalcCollider = isCalcCollider;
}

void Weapon::SetChargeRate(const float chargeRate)
{
	mChargeRate = chargeRate;
}