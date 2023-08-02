#include "DefaultCamera.h"

DefaultCamera::DefaultCamera()
{
}

void DefaultCamera::Init(Player* player)
{
	mPlayer = player;
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

	// �X�e�B�b�N�œ|���Ɖ�]���鏈��
	if (stick != 0)
	{
		controlYaw_ += stick.Norm().x * moveSpeed * rate.x;
		controlPitch_ += stick.Norm().y * moveSpeed * rate.y * 0.5f;
	}
	controlPitch_ = Clamp<float>(controlPitch_, -35, 80);

	// ���W
	const float angleOffset = 20.f;
	bool frontRange =
		mPlayer->GetRot().y <= camera_->rot.y + Radian(angleOffset) &&
		mPlayer->GetRot().y >= camera_->rot.y - Radian(angleOffset);
	bool backRange =
		mPlayer->GetRot().y <= camera_->rot.y + Radian(180) + Radian(angleOffset) &&
		mPlayer->GetRot().y >= camera_->rot.y + Radian(180) - Radian(angleOffset);

	if (!frontRange && !backRange)
	{
		Vec2 leftStick = Pad::GetStick(PadCode::LeftStick, 300);
		if (mPlayer->GetMoveVel() == 0)
		{
			leftStick = 0;
		}

		if (leftStick != 0)
		{
			targetYaw_ += leftStick.Norm().x;
		}

		assistYaw_ += (targetYaw_ - assistYaw_) * 0.05f;
	}

	// ���W�̐ݒ�
	vec_ =
	{
		-sinf(Radian(controlYaw_ + assistYaw_)) * cosf(Radian(controlPitch_)),
		+sinf(Radian(controlPitch_)),
		-cosf(Radian(controlYaw_ + assistYaw_)) * cosf(Radian(controlPitch_)),
	};

	const float length = 30.f;
	Vec3 curPos = mPlayer->GetPos() * Vec3(1.f, 0.f, 1.f) + Vec3(0.f, 9.5f, 0.f);
	camera_->pos = curPos + vec_.Norm() * length;

	// ��]�̏���
	rot_ =
	{
		Radian(controlPitch_),
		Radian(controlYaw_ + assistYaw_),
		0.f
	};

	// �p�x�̐ݒ�
	camera_->rot = rot_;

	if (isEase_ == true)
	{
		// �؂�ւ���Ƃ��ɃC�[�W���O���邽�߂̏���
		camera_->pos = Camera::current.pos;
		camera_->rot = Camera::current.rot;
		// ���݂̍��W (y���W�Œ�)
		Vec3 targetPos = curPos + -mPlayer->GetFrontVec() * length;
		Vec3 targetRot = { 0,mPlayer->GetRot().y,0 };

		// ���]���Ȃ��悤�ɂ��邽�߂̏���
		if (Camera::current.rot.y - targetRot.y >= Radian(180))
		{
			float diff = Radian(360) - camera_->rot.y;
			camera_->rot.y = -diff;
		}
		else if (Camera::current.rot.y - targetRot.y <= -Radian(180))
		{
			float diff = Radian(360) + camera_->rot.y;
			camera_->rot.y = diff;
		}

		targetPos_ = targetPos;
		targetRot_ = targetRot;

		EaseCamera();

		// ���݂��̃J�����ɑ��
		Camera::current = *camera_;
		targetYaw_ = 0;
		assistYaw_ = 0;
		controlPitch_ = Angle(Camera::current.rot.x);
		controlYaw_ = Angle(Camera::current.rot.y);
	}
	else
	{
		// ���݂��̃J�����ɑ��
		Camera::current = *camera_;
	}
}
