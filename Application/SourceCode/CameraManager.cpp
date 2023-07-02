#include "CameraManager.h"

CameraManager::CameraManager() :
	defCame_(std::make_unique<Camera>())
{
}

void CameraManager::Init()
{
	yaw_ = 0;		// ¶‰E‚Ì‰ñ“]Šp“x
	pitch_ = 0;		// ã‰º‚Ì‰ñ“]Šp“x
}

void CameraManager::Update()
{
	DefaultCameraUpdate();
}

void CameraManager::DefaultCameraUpdate()
{
	const float moveSpeed = 2.f;

	Vec2 stick = Pad::GetStick(PadCode::RightStick, 300);
	if (stick != 0)
	{
		yaw_ -= stick.Norm().x * moveSpeed;
		pitch_ -= stick.Norm().y * moveSpeed / 2.f;
	}

	const float length = 30.f;
	Vec3 v =
	{
		-sinf(Radian(yaw_)) * cosf(Radian(pitch_)),
		+sinf(Radian(pitch_)),
		-cosf(Radian(yaw_)) * cosf(Radian(pitch_)),
	};

	defCame_->pos = player_->GetHeadPos() + v.Norm() * length;

	Vec3 rot = { Radian(pitch_),Radian(yaw_),0.f };
	defCame_->rot = rot;

	Camera::current = *defCame_;
}

void CameraManager::SetPlayer(Player* player)
{
	player_ = player;
}