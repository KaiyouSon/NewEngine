#include "TargetCamera.h"

TargetCamera::TargetCamera()
{
}

void TargetCamera::Init(Player* player)
{
	player_ = player;
	isEaseEnd = false;
	camera_->pos = Camera::current.pos;
	camera_->rot = Camera::current.rot;

	const float length = 30.f;
	Vec3 target = Vec3(0.f, 7.125f, 20.f);
	Vec3 curPos = player_->GetPos();
	curPos.y = 9.5f;
	Vec3 vec = curPos - target;
	vec.y = 0;
	targetPos_ = curPos + vec.Norm() * length;

	Vec3 disToCamera = target - targetPos_;
	float pitch = atan2f(disToCamera.y, disToCamera.z);
	float yawcRad = atan2f(disToCamera.x, disToCamera.z);
	// ‰ñ“]‚Ìˆ—
	Vec3 rot =
	{
		//pitch,
		0.f,
		yawcRad,
		0.f,
	};
	// Šp“x‚Ìİ’è
	targetRot_ = rot;
}

void TargetCamera::Update()
{
	//if (isEaseEnd == false)
	//{
	EaseCamera();
	//}
	//else
	//{
	Vec3 target = Vec3(0.f, 7.125f, 20.f);

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
	float pitch = atan2f(disToCamera.y, disToCamera.z);
	float yawcRad = atan2f(disToCamera.x, disToCamera.z);

	// ‰ñ“]‚Ìˆ—
	Vec3 rot =
	{
		//pitch,
		0.f,
		yawcRad,
		0.f,
	};
	// Šp“x‚Ìİ’è
	camera_->rot = rot;
	//}

	// Œ»İ‚¢‚ÌƒJƒƒ‰‚É‘ã“ü
	Camera::current = *camera_;
}
