#include "IEffect.h"

void IEffect::SetisAcitve(const bool isActive)
{
	mIsActive = isActive;
}

bool IEffect::GetisActive()
{
	return mIsActive;
}

EffectType IEffect::GetEffectType()
{
	return mEffectType;
}
