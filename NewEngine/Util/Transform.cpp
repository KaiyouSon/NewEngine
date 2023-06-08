#include "Transform.h"
#include "MathUtil.h"

Transform::Transform() :
	pos_(0, 0, 0), scale_(1, 1, 1), rot_(0, 0, 0),
	worldMat_(Mat4::Identity()), scaleMat_(Mat4::Identity()),
	rotMat_(Mat4::Identity()), transMat_(Mat4::Identity()),
	billboardMat_(Mat4::Identity()), isUseBillboard_(false), billboardType_(XAxisBillboard)
{
}

Transform::Transform(const Vec3 pos, const Vec3 scale, const Vec3 rot) :
	pos_(pos), scale_(scale), rot_(rot),
	worldMat_(Mat4::Identity()), scaleMat_(Mat4::Identity()),
	rotMat_(Mat4::Identity()), transMat_(Mat4::Identity()),
	billboardMat_(Mat4::Identity()), isUseBillboard_(false), billboardType_(XAxisBillboard)
{
}

void Transform::Update()
{
	scaleMat_ = ConvertScalingMat(scale_);		 // スケーリング
	rotMat_ = Mat4::Identity();
	rotMat_ *= ConvertRotationZAxisMat(rot_.z); // z軸回転
	rotMat_ *= ConvertRotationXAxisMat(rot_.x); // x軸回転
	rotMat_ *= ConvertRotationYAxisMat(rot_.y); // y軸回転
	transMat_ = ConvertTranslationMat(pos_);	 // 平行移動

	BillBoardUpdate();

	worldMat_ = Mat4::Identity();
	worldMat_ *= billboardMat_;
	worldMat_ *= scaleMat_;
	worldMat_ *= rotMat_;
	worldMat_ *= transMat_;
}

void Transform::BillBoardUpdate()
{
	if (isUseBillboard_ == false)
	{
		billboardMat_ = Mat4::Identity();
	}
	else
	{
		switch (billboardType_)
		{
		case XAxisBillboard:
		{
			billboardMat_ = Mat4::Identity();
			billboardMat_ *= ConvertBillBoardXAxis();
			break;
		}
		case YAxisBillboard:
		{
			billboardMat_ = Mat4::Identity();
			billboardMat_ *= ConvertBillBoardYAxis();
			break;
		}
		case ZAxisBillboard:
		{
			billboardMat_ = Mat4::Identity();
			billboardMat_ *= ConvertBillBoardZAxis();
			break;
		}
		case AllAxisBillboard:
		{
			billboardMat_ = Mat4::Identity();
			billboardMat_ *= ConvertBillBoardAllAxis();
			break;
		}
		default:
			break;
		}
	}
}

#pragma region セッター

Mat4 Transform::GetTransMat() { return transMat_; }	// 移動行列
Mat4 Transform::GetScaleMat() { return scaleMat_; }	// スケール行列
Mat4 Transform::GetRotMat() { return rotMat_; }		// 回転行列
Mat4 Transform::GetWorldMat() { return worldMat_; }	// ワールド行列

#pragma endregion

#pragma region セッター

void Transform::SetWorldMat(Mat4 worldMat) { worldMat_ = worldMat; }

#pragma endregion

