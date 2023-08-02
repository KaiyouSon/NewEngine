#include "ICamera.h"

ICamera::ICamera() : mCamera(std::make_unique<Camera>())
{
}

void ICamera::EaseCamera()
{
	mCamera->pos += (mTargetPos - mCamera->pos) * 0.2f;
	mCamera->rot += (mTargetRot - mCamera->rot) * 0.2f;

	if (Absolut(mTargetPos - mCamera->pos) <= 0.1f)
	{
		mIsEase = false;
	}
}

void ICamera::SetLockonPos(const Vec3 pos)
{
	mLockonPos = pos;
}

void ICamera::SetisEase(const bool isEase)
{
	mIsEase = isEase;
}

Vec3 ICamera::GetLockonPos()
{
	return mLockonPos;
}
