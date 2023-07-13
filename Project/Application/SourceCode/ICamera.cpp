#include "ICamera.h"

ICamera::ICamera() :camera_(std::make_unique<Camera>())
{
}

void ICamera::EaseCamera()
{
	camera_->pos += (targetPos_ - camera_->pos) * 0.2f;
	camera_->rot += (targetRot_ - camera_->rot) * 0.2f;

	if (Absolut(targetPos_ - camera_->pos) <= 0.1f)
	{
		isEase_ = false;
	}
}

void ICamera::SetLockonPos(const Vec3 pos)
{
	lockonPos_ = pos;
}

void ICamera::SetisEase(const bool isEase)
{
	isEase_ = isEase;
}

Vec3 ICamera::GetLockonPos()
{
	return lockonPos_;
}
