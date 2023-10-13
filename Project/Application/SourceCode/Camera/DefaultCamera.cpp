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

	// 繧ｹ繝・ぅ繝・け縺ｧ蛟偵☆縺ｨ蝗櫁ｻ｢縺吶ｋ蜃ｦ逅・
	if (stick != 0)
	{
		mControlYaw += stick.Norm().x * moveSpeed * rate.x;
		mControlPitch += stick.Norm().y * moveSpeed * rate.y * 0.5f;
	}
	mControlPitch = Clamp<float>(mControlPitch, -35, 80);

	// 蠎ｧ讓・
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

	// 蠎ｧ讓吶・險ｭ螳・
	mVec =
	{
		-sinf(Radian(mControlYaw + mAssistYaw)) * cosf(Radian(mControlPitch)),
		+sinf(Radian(mControlPitch)),
		-cosf(Radian(mControlYaw + mAssistYaw)) * cosf(Radian(mControlPitch)),
	};

	const float length = 30.f;
	Vec3 curPos = mPlayer->GetPos() * Vec3(1.f, 0.f, 1.f) + Vec3(0.f, 9.5f, 0.f);
	mCamera->pos = curPos + mVec.Norm() * length;

	// 蝗櫁ｻ｢縺ｮ蜃ｦ逅・
	mRot =
	{
		Radian(mControlPitch),
		Radian(mControlYaw + mAssistYaw),
		0.f
	};

	// 隗貞ｺｦ縺ｮ險ｭ螳・
	mCamera->rot = mRot;

	if (mIsEase == true)
	{
		// 蛻・ｊ譖ｿ縺医ｋ縺ｨ縺阪↓繧､繝ｼ繧ｸ繝ｳ繧ｰ縺吶ｋ縺溘ａ縺ｮ蜃ｦ逅・
		mCamera->pos = Camera::current.pos;
		mCamera->rot = Camera::current.rot;
		// 迴ｾ蝨ｨ縺ｮ蠎ｧ讓・(y蠎ｧ讓吝崋螳・
		Vec3 targetPos = curPos + -mPlayer->GetFrontVec() * length;
		Vec3 targetRot = { 0,mPlayer->GetRot().y,0 };

		// 荳蝗櫁ｻ｢縺励↑縺・ｈ縺・↓縺吶ｋ縺溘ａ縺ｮ蜃ｦ逅・
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

		// 迴ｾ蝨ｨ縺・・繧ｫ繝｡繝ｩ縺ｫ莉｣蜈･
		Camera::current = *mCamera;
		mTargetYaw = 0;
		mAssistYaw = 0;
		mControlPitch = Angle(Camera::current.rot.x);
		mControlYaw = Angle(Camera::current.rot.y);
	}
	else
	{
		// 迴ｾ蝨ｨ縺・・繧ｫ繝｡繝ｩ縺ｫ莉｣蜈･
		Camera::current = *mCamera;
	}
}

