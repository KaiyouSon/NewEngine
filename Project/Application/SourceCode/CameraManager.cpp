#include "CameraManager.h"

CameraManager::CameraManager() :
	defCame_(std::make_unique<Camera>())
{
	finalVec_ = { 0,0,-1 };
}

void CameraManager::Init()
{
	yaw_ = 0;		// ���E�̉�]�p�x
	pitch_ = 0;		// �㉺�̉�]�p�x
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

	// �X�e�B�b�N�œ|���Ɖ�]���鏈��
	if (stick != 0)
	{
		controlYaw_ += stick.Norm().x * moveSpeed * rate.x;
		controlPitch_ += stick.Norm().y * moveSpeed * rate.y * 0.5f;
	}
	pitch_ = Clamp<float>(pitch_, -35, 80);

	// ���W
	const float angleOffset = 30.f;
	bool frontRange =
		player_->GetRot().y <= defCame_->rot.y + Radian(angleOffset) &&
		player_->GetRot().y >= defCame_->rot.y - Radian(angleOffset);
	bool backRange =
		fabsf(player_->GetRot().y) <= fabsf(defCame_->rot.y + Radian(180) + Radian(angleOffset)) &&
		fabsf(player_->GetRot().y) >= fabsf(defCame_->rot.y + Radian(180) - Radian(angleOffset));

	if (!frontRange && !backRange)
	{
		Vec2 leftStick = Pad::GetStick(PadCode::LeftStick, 300);
		if (leftStick != 0)
		{
			targetYaw_ += leftStick.Norm().x;
		}

		assistYaw_ += (targetYaw_ - assistYaw_) * 0.05f;
	}

	// ���W�̐ݒ�
	finalVec_ =
	{
		-sinf(Radian(controlYaw_ + assistYaw_)) * cosf(Radian(controlPitch_)),
		+sinf(Radian(controlPitch_)),
		-cosf(Radian(controlYaw_ + assistYaw_)) * cosf(Radian(controlPitch_)),
	};

	const float length = 30.f;
	Vec3 curPos = player_->GetPos();
	curPos.y = 9.5f;
	defCame_->pos = curPos + finalVec_.Norm() * length;


	// ��]�̏���
	finalRot_ =
	{
		Radian(controlPitch_),
		Radian(controlYaw_ + assistYaw_),
		0.f
	};

	// �p�x�̐ݒ�
	defCame_->rot = finalRot_;

	// ���݂��̃J�����ɑ��
	Camera::current = *defCame_;
}

void CameraManager::SetPlayer(Player* player)
{
	player_ = player;
}