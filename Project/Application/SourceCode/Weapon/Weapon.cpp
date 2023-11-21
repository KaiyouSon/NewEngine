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

void Weapon::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	weapon->SetGraphicsPipeline(graphicsPipeline);
}

void Weapon::SetisActiveTrajectory(const bool isActiveTrajectory)
{
	mIsActiveTrajectory = isActiveTrajectory;
}

void Weapon::SetisShadow(const bool isShadow)
{
	weapon->SetisShadow(false, isShadow);
}
