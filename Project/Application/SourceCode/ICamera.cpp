#include "ICamera.h"

ICamera::ICamera() :camera_(std::make_unique<Camera>())
{
}

void ICamera::EaseCamera()
{
	camera_->pos += (targetPos_ - camera_->pos) * 0.2f;
	camera_->rot += (targetRot_ - camera_->rot) * 0.2f;

	if (targetPos_ - camera_->pos <= 0.1f)
	{
		isEaseEnd = true;
	}

}