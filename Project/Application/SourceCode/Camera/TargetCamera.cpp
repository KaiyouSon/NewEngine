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

	// 座標の設定
	const float length = 30.f;	// 長さ
	// 現在の座標 (y座標固定)
	Vec3 curPos = mPlayer->GetPos() * Vec3(1.f, 0.f, 1.f) + Vec3(0.f, 9.5f, 0.f);
	// ターゲットからのベクトル (ターゲットの高さに依存しないため、y軸を無視する)
	Vec3 vec = (curPos - target) * Vec3(1.f, 0.f, 1.f);

	const float pitchRad = Radian(22.5f);

	// 右ベクトルを基準に回転する(上から見下ろすようにするため)
	Quaternion q = vec;
	Vec3 rightVec = Vec3::Cross(vec, Vec3::up);
	vec = q.AnyAxisRotation(rightVec, pitchRad);

	mTargetCamePos = curPos + vec.Norm() * length;
	mCurCamePos += (mTargetCamePos - mCamera->pos) * 0.25f;
	mCamera->pos = mCurCamePos;

	Vec3 disToCamera = target - curPos;
	float yawRad = atan2f(disToCamera.x, disToCamera.z);

	// 回転の処理
	Vec3 rot =
	{
		pitchRad,
		yawRad,
		0.f,
	};
	// 角度の設定
	mCamera->rot = rot;

	// 切り替えるときにイージングするための処理
	mCamera->pos = Camera::current.pos;
	mCamera->rot = Camera::current.rot;

	// 一回転しないようにするための処理
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

	// 現在いのカメラに代入
	Camera::current = *mCamera;
}
