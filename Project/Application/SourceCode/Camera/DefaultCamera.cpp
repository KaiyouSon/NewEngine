#include "DefaultCamera.h"

void DefaultCamera::Init(Player* player)
{
	mPlayer = player;
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

	const Vec2 rate = (Absolute(stick) - deadZone) / (Absolute(max) - deadZone);

	// カメラのヨー軸とピッチ軸を制御する
	if (stick != 0)
	{
		mControlYaw += stick.Norm().x * moveSpeed * rate.x;
		mControlPitch += stick.Norm().y * moveSpeed * rate.y * 0.5f;
	}
	mControlPitch = Clamp<float>(mControlPitch, -35, 80);

	// 方向ベクトル
	mVec =
	{
		-sinf(Radian(mControlYaw)) * cosf(Radian(mControlPitch)),
		+sinf(Radian(mControlPitch)),
		-cosf(Radian(mControlYaw)) * cosf(Radian(mControlPitch)),
	};

	const float length = 40.f;
	Vec3 curPos = mPlayer->GetPos() * Vec3(1.f, 0.f, 1.f) + Vec3(0.f, 9.5f, 0.f);
	mCamera->pos = curPos + mVec.Norm() * length;

	// 方向転換角度
	mRot =
	{
		Radian(mControlPitch),
		Radian(mControlYaw),
		0.f
	};

	// カメラの回転
	mCamera->rot = mRot;

	if (mIsEase == true)
	{
		// イーズアウト処理中はカメラ位置と回転をスムーズに変更
		mCamera->pos = Camera::current.pos;
		mCamera->rot = Camera::current.rot;

		// 追従対象位置と角度 (y軸方向は追従しない)
		Vec3 targetPos = curPos + -mPlayer->GetFrontVec() * length;
		Vec3 targetRot = { 0, mPlayer->GetRot().y, 0 };

		// カメラ位置の補正
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

		// イーズアウト処理後は現在のカメラを更新
		Camera::current = *mCamera;
		mControlPitch = Angle(Camera::current.rot.x);
		mControlYaw = Angle(Camera::current.rot.y);
	}
	else
	{
		// イーズアウト処理後は現在のカメラを更新
		Camera::current = *mCamera;
	}
}