#include "IWeaponMotion.h"

void IWeaponMotion::SetisPlay(const bool isPlay)
{
	isPlay_ = isPlay;
}

bool IWeaponMotion::GetisPlay()
{
	return isPlay_;
}