#include "ICamera.h"

ICamera::ICamera() :camera_(std::make_unique<Camera>())
{
}

void ICamera::EaseCamera()
{
	camera_->pos += (targetPos_ - camera_->pos) * 0.1f;
	camera_->rot += (targetRot_ - camera_->rot) * 0.1f;

	if (targetPos_ - camera_->pos <= 0.01f)
	{
		isEaseEnd = true;
	}

}