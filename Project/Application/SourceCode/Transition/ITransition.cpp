#include "ITransition.h"

TransitionType ITransition::GetType()
{
	return mType;
}

TransitionStep ITransition::GetStep()
{
	return mStep;
}

void ITransition::SetStep(const TransitionStep step)
{
	mStep = step;
}
