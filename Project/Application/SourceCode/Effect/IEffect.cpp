#include "IEffect.h"

bool IEffect::GetisActive()
{
	return mIsActive;
}

EffectType IEffect::GetEffectType()
{
	return mEffectType;
}
