#include "Transform.h"
#include "MathUtil.h"

Transform::Transform() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	worldMat_(Mat4::Identity()), scaleMat_(Mat4::Identity()),
	rotMat_(Mat4::Identity()), transMat_(Mat4::Identity())
{
}

Transform::Transform(const Vec3 pos, const Vec3 scale, const Vec3 rot) :
	pos(pos), scale(scale), rot(rot),
	worldMat_(Mat4::Identity()), scaleMat_(Mat4::Identity()),
	rotMat_(Mat4::Identity()), transMat_(Mat4::Identity())
{
}

void Transform::Update()
{
	scaleMat_ = ConvertScalingMat(scale);		 // スケーリング
	rotMat_ = Mat4::Identity();
	rotMat_ *= ConvertRotationZAxisMat(rot.z); // z軸回転
	rotMat_ *= ConvertRotationXAxisMat(rot.x); // x軸回転
	rotMat_ *= ConvertRotationYAxisMat(rot.y); // y軸回転
	transMat_ = ConvertTranslationMat(pos);	 // 平行移動

	// ビルボード行列計算
	billboard_.CalculateBillboardMat();

	// ワールド行列計算
	worldMat_ = Mat4::Identity();

	if (billboard_.GetBillboardType() != BillboardType::None)
	{
		worldMat_ *= billboard_.GetMat();
	}

	worldMat_ *= scaleMat_;
	worldMat_ *= rotMat_;
	worldMat_ *= transMat_;
}

#pragma region セッター

Mat4 Transform::GetTransMat() { return transMat_; }	// 移動行列
Mat4 Transform::GetScaleMat() { return scaleMat_; }	// スケール行列
Mat4 Transform::GetRotMat() { return rotMat_; }		// 回転行列
Mat4 Transform::GetWorldMat() { return worldMat_; }	// ワールド行列

#pragma endregion

#pragma region セッター

void Transform::SetWorldMat(Mat4 worldMat) { worldMat_ = worldMat; }

void Transform::SetBillboardType(const BillboardType type) { billboard_.SetBillboardType(type); }

#pragma endregion

