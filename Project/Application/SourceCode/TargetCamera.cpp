#include "TargetCamera.h"
#include "CameraManager.h"

TargetCamera::TargetCamera()
{
}

void TargetCamera::Init(Player* player)
{
	player_ = player;
	isEaseEnd = false;
}

void TargetCamera::Update()
{
	Vec3 target = lockonPos_;

	// À•W‚Ìİ’è
	const float length = 30.f;
	Vec3 curPos = player_->GetPos();
	curPos.y = 9.5f;
	Vec3 vec = curPos - target;
	vec.y = 0;
	targetCamePos_ = curPos + vec.Norm() * length;
	curCamePos_ += (targetCamePos_ - camera_->pos) * 0.25f;
	camera_->pos = curCamePos_;

	Vec3 disToCamera = target - curPos;
	float yawRad = atan2f(disToCamera.x, disToCamera.z);

	// ‰ñ“]‚Ìˆ—
	Vec3 rot =
	{
		0.f,
		yawRad,
		0.f,
	};
	// Šp“x‚Ìİ’è
	camera_->rot = rot;

	camera_->pos = Camera::current.pos;
	camera_->rot = Camera::current.rot;

	if (Camera::current.rot.y - rot.y >= Radian(180))
	{
		float diff = Radian(360) - camera_->rot.y;
		camera_->rot.y = -diff;
	}
	else if (Camera::current.rot.y - rot.y <= -Radian(180))
	{
		float diff = Radian(360) + camera_->rot.y;
		camera_->rot.y = diff;
	}

	// ˆê‰ñ“]‚µ‚È‚¢‚æ‚¤‚É‚·‚é‚½‚ß‚Ìˆ—
	//if (camera_->rot.y >= Radian(180))
	//{
	//	float diff = Radian(360) - camera_->rot.y;
	//	camera_->rot.y = -diff;
	//}
	//else if (camera_->rot.y <= -Radian(180))
	//{
	//	float diff = Radian(360) + camera_->rot.y;
	//	camera_->rot.y = diff;
	//}
	targetPos_ = targetCamePos_;
	targetRot_ = rot;
	EaseCamera();

	// Œ»İ‚¢‚ÌƒJƒƒ‰‚É‘ã“ü
	Camera::current = *camera_;
}
