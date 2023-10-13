#include "TargetCamera.h"
#include "CameraManager.h"

TargetCamera::TargetCamera()
{
}

void TargetCamera::Init(Player* player)
{
	mPlayer = player;
	mIsEase = false;
}

void TargetCamera::Update()
{
	Vec3 target = mLockonPos;

	// 蠎ｧ讓吶・險ｭ螳・
	const float length = 30.f;	// 髟ｷ縺・
	// 迴ｾ蝨ｨ縺ｮ蠎ｧ讓・(y蠎ｧ讓吝崋螳・
	Vec3 curPos = mPlayer->GetPos() * Vec3(1.f, 0.f, 1.f) + Vec3(0.f, 9.5f, 0.f);
	// 繧ｿ繝ｼ繧ｲ繝・ヨ縺九ｉ縺ｮ繝吶け繝医Ν (繧ｿ繝ｼ繧ｲ繝・ヨ縺ｮ鬮倥＆縺ｫ萓晏ｭ倥＠縺ｪ縺・◆繧√【霆ｸ繧堤┌隕悶☆繧・
	Vec3 vec = (curPos - target) * Vec3(1.f, 0.f, 1.f);

	const float pitchRad = Radian(22.5f);

	// 蜿ｳ繝吶け繝医Ν繧貞渕貅悶↓蝗櫁ｻ｢縺吶ｋ(荳翫°繧芽ｦ倶ｸ九ｍ縺吶ｈ縺・↓縺吶ｋ縺溘ａ)
	Quaternion q = vec;
	Vec3 rightVec = Vec3::Cross(vec, Vec3::up);
	vec = q.AnyAxisRotation(rightVec, pitchRad);

	mTargetCamePos = curPos + vec.Norm() * length;
	mCurCamePos += (mTargetCamePos - mCamera->pos) * 0.25f;
	mCamera->pos = mCurCamePos;

	Vec3 disToCamera = target - curPos;
	float yawRad = atan2f(disToCamera.x, disToCamera.z);

	// 蝗櫁ｻ｢縺ｮ蜃ｦ逅・
	Vec3 rot =
	{
		pitchRad,
		yawRad,
		0.f,
	};
	// 隗貞ｺｦ縺ｮ險ｭ螳・
	mCamera->rot = rot;

	// 蛻・ｊ譖ｿ縺医ｋ縺ｨ縺阪↓繧､繝ｼ繧ｸ繝ｳ繧ｰ縺吶ｋ縺溘ａ縺ｮ蜃ｦ逅・
	mCamera->pos = Camera::current.pos;
	mCamera->rot = Camera::current.rot;

	// 荳蝗櫁ｻ｢縺励↑縺・ｈ縺・↓縺吶ｋ縺溘ａ縺ｮ蜃ｦ逅・
	if (Camera::current.rot.y - rot.y >= Radian(180))
	{
		float diff = Radian(360) - mCamera->rot.y;
		mCamera->rot.y = -diff;
	}
	else if (Camera::current.rot.y - rot.y <= -Radian(180))
	{
		float diff = Radian(360) + mCamera->rot.y;
		mCamera->rot.y = diff;
	}

	mTargetPos = mTargetCamePos;
	mTargetRot = rot;
	EaseCamera();

	// 迴ｾ蝨ｨ縺・・繧ｫ繝｡繝ｩ縺ｫ莉｣蜈･
	Camera::current = *mCamera;
}

