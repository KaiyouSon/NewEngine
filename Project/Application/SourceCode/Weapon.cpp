#include "Weapon.h"

float Weapon::GetDamage()
{
	return damage_;
}

float Weapon::GetChargeRate()
{
	return chargeRate_;
}

void Weapon::SetisCalcCollider(const bool isCalcCollider)
{
	isCalcCollider_ = isCalcCollider;
}

void Weapon::SetChargeRate(const float chargeRate)
{
	chargeRate_ = chargeRate;
}