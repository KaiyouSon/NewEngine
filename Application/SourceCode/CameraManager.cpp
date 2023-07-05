#include "CameraManager.h"

CameraManager::CameraManager() :
	defCame_(std::make_unique<Camera>())
{
}

void CameraManager::Init()
{
	yaw_ = 0;		// ç∂âEÇÃâÒì]äpìx
	pitch_ = 0;		// è„â∫ÇÃâÒì]äpìx
}

void CameraManager::Update()
{
	DefaultCameraUpdate();
}

void CameraManager::DefaultCameraUpdate()
{
	float deadZone = 100;
	const float max = 1000;

	const float moveSpeed = 4.f;
	Vec2 stick = Pad::GetStick(PadCode::RightStick, deadZone);
	stick.x = Clamp<float>(stick.x, -max, max);
	stick.y = Clamp<float>(stick.y, -max, max);

	const Vec2 rate = (Absolut(stick) - deadZone) / (Absolut(max) - deadZone);

	if (stick != 0)
	{
		yaw_ += stick.Norm().x * moveSpeed * rate.x;
		pitch_ += stick.Norm().y * moveSpeed * rate.y * 0.5f;
	}
	pitch_ = Clamp<float>(pitch_, -35, 80);

	const float length = 25.f;
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