#include "DefaultCamera.h"

DefaultCamera::DefaultCamera()
{
}

void DefaultCamera::Init(Player* player)
{
	player_ = player;
	assistYaw_ = 0;
	controlPitch_ = Angle(Camera::current.rot.x);
	controlYaw_ = Angle(Camera::current.rot.y);

	vec_ = Vec3::back;
	rot_ = 0;

	camera_->pos = Camera::current.pos;
}

void DefaultCamera::Update()
{
	float deadZone = 100;
	const float max = 1000;

	const float moveSpeed = 4.f;
	Vec2 stick = Pad::GetStick(PadCode::RightStick, deadZone);

	stick.x = Clamp<float>(stick.x, -max, max);
	stick.y = Clamp<float>(stick.y, -max, max);

	const Vec2 rate = (Absolut(stick) - deadZone) / (Absolut(max) - deadZone);

	// スティックで倒すと回転する処理
	if (stick != 0)
	{
		controlYaw_ += stick.Norm().x * moveSpeed * rate.x;
		controlPitch_ += stick.Norm().y * moveSpeed * rate.y * 0.5f;
	}
	controlPitch_ = Clamp<float>(controlPitch_, -35, 80);

	// 座標
	const float angleOffset = 10.f;
	bool frontRange =
		player_->GetRot().y <= camera_->rot.y + Radian(angleOffset) &&
		player_->GetRot().y >= camera_->rot.y - Radian(angleOffset);
	bool backRange =
		player_->GetRot().y <= camera_->rot.y + Radian(180) + Radian(angleOffset) &&
		player_->GetRot().y >= camera_->rot.y + Radian(180) - Radian(angleOffset);

	if (!frontRange && !backRange)
	{
		Vec2 leftStick = Pad::GetStick(PadCode::LeftStick, 300);
		if (leftStick != 0)
		{
			targetYaw_ += leftStick.Norm().x;
		}

		assistYaw_ += (targetYaw_ - assistYaw_) * 0.05f;
	}

	// 座標の設定
	vec_ =
	{
		-sinf(Radian(controlYaw_ + assistYaw_)) * cosf(Radian(controlPitch_)),
		+sinf(Radian(controlPitch_)),
		-cosf(Radian(controlYaw_ + assistYaw_)) * cosf(Radian(controlPitch_)),
	};

	const float length = 30.f;
	Vec3 curPos = player_->GetPos();
	curPos.y = 9.5f;
	camera_->pos = curPos + vec_.Norm() * length;

	// 回転の処理
	rot_ =
	{
		Radian(controlPitch_),
		Radian(controlYaw_ + assistYaw_),
		0.f
	};

	// 角度の設定
	camera_->rot = rot_;

	// 現在いのカメラに代入
	Camera::current = *camera_;
}
