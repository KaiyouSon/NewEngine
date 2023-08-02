#include "DefaultCamera.h"

DefaultCamera::DefaultCamera()
{
}

void DefaultCamera::Init(Player* player)
{
	mPlayer = player;
	mAssistYaw = 0;
	mControlPitch = Angle(Camera::current.rot.x);
	mControlYaw = Angle(Camera::current.rot.y);

	mVec = Vec3::back;
	mRot = 0;

	mCamera->pos = Camera::current.pos;
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
		mControlYaw += stick.Norm().x * moveSpeed * rate.x;
		mControlPitch += stick.Norm().y * moveSpeed * rate.y * 0.5f;
	}
	mControlPitch = Clamp<float>(mControlPitch, -35, 80);

	// ���W
	const float angleOffset = 20.f;
	bool frontRange =
		mPlayer->GetRot().y <= mCamera->rot.y + Radian(angleOffset) &&
		mPlayer->GetRot().y >= mCamera->rot.y - Radian(angleOffset);
	bool backRange =
		mPlayer->GetRot().y <= mCamera->rot.y + Radian(180) + Radian(angleOffset) &&
		mPlayer->GetRot().y >= mCamera->rot.y + Radian(180) - Radian(angleOffset);

	if (!frontRange && !backRange)
	{
		Vec2 leftStick = Pad::GetStick(PadCode::LeftStick, 300);
		if (mPlayer->GetMoveVel() == 0)
		{
			leftStick = 0;
		}

		if (leftStick != 0)
		{
			mTargetYaw += leftStick.Norm().x;
		}

		mAssistYaw += (mTargetYaw - mAssistYaw) * 0.05f;
	}

	// ���W�̐ݒ�
	mVec =
	{
		-sinf(Radian(mControlYaw + mAssistYaw)) * cosf(Radian(mControlPitch)),
		+sinf(Radian(mControlPitch)),
		-cosf(Radian(mControlYaw + mAssistYaw)) * cosf(Radian(mControlPitch)),
	};

	const float length = 30.f;
	Vec3 curPos = mPlayer->GetPos() * Vec3(1.f, 0.f, 1.f) + Vec3(0.f, 9.5f, 0.f);
	mCamera->pos = curPos + mVec.Norm() * length;

	// ��]�̏���
	mRot =
	{
		Radian(mControlPitch),
		Radian(mControlYaw + mAssistYaw),
		0.f
	};

	// �p�x�̐ݒ�
	mCamera->rot = mRot;

	if (mIsEase == true)
	{
		// �؂�ւ���Ƃ��ɃC�[�W���O���邽�߂̏���
		mCamera->pos = Camera::current.pos;
		mCamera->rot = Camera::current.rot;
		// ���݂̍��W (y���W�Œ�)
		Vec3 targetPos = curPos + -mPlayer->GetFrontVec() * length;
		Vec3 targetRot = { 0,mPlayer->GetRot().y,0 };

		// ���]���Ȃ��悤�ɂ��邽�߂̏���
		if (Camera::current.rot.y - targetRot.y >= Radian(180))
		{
			float diff = Radian(360) - mCamera->rot.y;
			mCamera->rot.y = -diff;
		}
		else if (Camera::current.rot.y - targetRot.y <= -Radian(180))
		{
			float diff = Radian(360) + mCamera->rot.y;
			mCamera->rot.y = diff;
		}

		mTargetPos = targetPos;
		mTargetRot = targetRot;

		EaseCamera();

		// ���݂��̃J�����ɑ��
		Camera::current = *mCamera;
		mTargetYaw = 0;
		mAssistYaw = 0;
		mControlPitch = Angle(Camera::current.rot.x);
		mControlYaw = Angle(Camera::current.rot.y);
	}
	else
	{
		// ���݂��̃J�����ɑ��
		Camera::current = *mCamera;
	}
}
