#include "Transform.h"
#include "MathUtil.h"

Transform::Transform() :
    pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
    mWorldMat(Mat4::Identity()), mScaleMat(Mat4::Identity()),
    mRotMat(Mat4::Identity()), mTransMat(Mat4::Identity())
{
}

Transform::Transform(const Vec3 pos, const Vec3 scale, const Vec3 rot) :
    pos(pos), scale(scale), rot(rot),
    mWorldMat(Mat4::Identity()), mScaleMat(Mat4::Identity()),
    mRotMat(Mat4::Identity()), mTransMat(Mat4::Identity())
{
}

void Transform::Update()
{
    mScaleMat = ConvertScalingMat(scale); // スケーリング行列
    mRotMat = Mat4::Identity();
    mRotMat *= ConvertRotationZAxisMat(rot.z); // Z軸回転行列
    mRotMat *= ConvertRotationXAxisMat(rot.x); // X軸回転行列
    mRotMat *= ConvertRotationYAxisMat(rot.y); // Y軸回転行列
    mTransMat = ConvertTranslationMat(pos);    // 位置行列

    // ビルボード行列の計算
    mBillboard.CalculateBillboardMat();

    // ワールド行列の計算
    mWorldMat = Mat4::Identity();

    if (mBillboard.GetBillboardType() != BillboardType::None)
    {
        mWorldMat *= mBillboard.GetMat();
    }

    mWorldMat *= mScaleMat;
    mWorldMat *= mRotMat;
    mWorldMat *= mTransMat;
}

// 以下、行列の取得関数
Mat4 Transform::GetTransMat() { return mTransMat; }    // 位置行列
Mat4 Transform::GetScaleMat() { return mScaleMat; }    // スケーリング行列
Mat4 Transform::GetRotMat() { return mRotMat; }        // 回転行列
Mat4 Transform::GetWorldMat() { return mWorldMat; }    // ワールド行列

// ビルボード設定関数
void Transform::SetWorldMat(Mat4 worldMat) { mWorldMat = worldMat; }
void Transform::SetBillboardType(const BillboardType type) { mBillboard.SetBillboardType(type); }