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
	mScaleMat = ConvertScalingMat(scale); // スケーリング
	mRotMat = Mat4::Identity();
	mRotMat *= ConvertRotationZAxisMat(rot.z); // z軸回転
	mRotMat *= ConvertRotationXAxisMat(rot.x); // x軸回転
	mRotMat *= ConvertRotationYAxisMat(rot.y); // y軸回転
	mTransMat = ConvertTranslationMat(pos);	 // 平行移動

	// ビルボード行列計算
	mBillboard.CalculateBillboardMat();

	// ワールド行列計算
	mWorldMat = Mat4::Identity();

	if (mBillboard.GetBillboardType() != BillboardType::None)
	{
		mWorldMat *= mBillboard.GetMat();
	}

	mWorldMat *= mScaleMat;
	mWorldMat *= mRotMat;
	mWorldMat *= mTransMat;
}

#pragma region セッター

Mat4 Transform::GetTransMat() { return mTransMat; }	// 移動行列
Mat4 Transform::GetScaleMat() { return mScaleMat; }	// スケール行列
Mat4 Transform::GetRotMat() { return mRotMat; }		// 回転行列
Mat4 Transform::GetWorldMat() { return mWorldMat; }	// ワールド行列

#pragma endregion

#pragma region セッター

void Transform::SetWorldMat(Mat4 worldMat) { mWorldMat = worldMat; }

void Transform::SetBillboardType(const BillboardType type) { mBillboard.SetBillboardType(type); }

#pragma endregion

