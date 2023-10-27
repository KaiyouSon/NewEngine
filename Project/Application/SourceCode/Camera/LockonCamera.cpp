#include "LockonCamera.h"
#include "CameraManager.h"

LockonCamera::LockonCamera()
{
}

void LockonCamera::Init(Player* player)
{
	mPlayer = player;
	mIsEase = false;
}

void LockonCamera::Update()
{
    Vec3 target = mLockonPos;

    // 方向転換
    const float length = 30.f; // 距離
    // 追従対象位置 (y軸方向は追従しない)
    Vec3 curPos = mPlayer->GetPos() * Vec3(1.f, 0.f, 1.f) + Vec3(0.f, 9.5f, 0.f);
    // 方向ベクトル
    Vec3 vec = (curPos - target) * Vec3(1.f, 0.f, 1.f);

    const float pitchRad = Radian(22.5f);

    // カメラ位置の補正
    Quaternion q = vec;
    Vec3 rightVec = Vec3::Cross(vec, Vec3::up);
    vec = q.AnyAxisRotation(rightVec, pitchRad);

    mTargetCamePos = curPos + vec.Norm() * length;
    mCurCamePos += (mTargetCamePos - mCamera->pos) * 0.25f;
    mCamera->pos = mCurCamePos;

    Vec3 disToCamera = target - curPos;
    float yawRad = atan2f(disToCamera.x, disToCamera.z);

    // カメラ回転
    Vec3 rot =
    {
        pitchRad,
        yawRad,
        0.f,
    };
    // 方向転換角度
    mCamera->rot = rot;

    // イーズアウト処理中はカメラ位置と回転をスムーズに変更
    mCamera->pos = Camera::current.pos;
    mCamera->rot = Camera::current.rot;

    // カメラ位置の補正
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

    // イーズアウト処理後は現在のカメラを更新
    Camera::current = *mCamera;
}
