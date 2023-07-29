#include "Weapon.h"

float Weapon::GetDamage()
{
	return damage_;
}

void Weapon::SetisCalcCollider(const bool isCalcCollider)
{
	isCalcCollider_ = isCalcCollider;
}
