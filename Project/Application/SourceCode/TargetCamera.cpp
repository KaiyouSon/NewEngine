#include "TargetCamera.h"

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

	// 座標の設定
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

	// 回転の処理
	Vec3 rot =
	{
		0.f,
		yawRad,
		0.f,
	};
	// 角度の設定
	camera_->rot = rot;

	camera_->pos = Camera::current.pos;
	camera_->rot = Camera::current.rot;
	// 一回転しないようにするための処理
	if (camera_->rot.y >= Radian(180))
	{
		float diff = Radian(360) - camera_->rot.y;
		camera_->rot.y = -diff;
	}
	else if (camera_->rot.y <= -Radian(180))
	{
		float diff = Radian(360) + camera_->rot.y;
		camera_->rot.y = diff;
	}
	targetPos_ = targetCamePos_;
	targetRot_ = rot;
	EaseCamera();

	// 現在いのカメラに代入
	Camera::current = *camera_;
}
