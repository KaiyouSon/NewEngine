#include "Transform.h"
#include "MathUtil.h"

Transform::Transform() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	worldMat_(Mat4::Identity()), scaleMat(Mat4::Identity()),
	rotMat(Mat4::Identity()), transMat(Mat4::Identity()),
	billboardMat(Mat4::Identity()), isUseBillboard(false), billboardType(XAxisBillboard)
{
}

Transform::Transform(const Vec3& pos, const Vec3& scale, const Vec3& rot) :
	pos(pos), scale(scale), rot(rot),
	worldMat_(Mat4::Identity()), scaleMat(Mat4::Identity()),
	rotMat(Mat4::Identity()), transMat(Mat4::Identity()),
	billboardMat(Mat4::Identity()), isUseBillboard(false), billboardType(XAxisBillboard)
{
}

void Transform::Update()
{
	scaleMat = ConvertScalingMat(scale);		 // スケーリング
	rotMat = Mat4::Identity();
	rotMat *= ConvertRotationZAxisMat(rot.z); // z軸回転
	rotMat *= ConvertRotationXAxisMat(rot.x); // x軸回転
	rotMat *= ConvertRotationYAxisMat(rot.y); // y軸回転
	transMat = ConvertTranslationMat(pos);	 // 平行移動

	BillBoardUpdate();

	worldMat_ = Mat4::Identity();
	worldMat_ *= billboardMat;
	worldMat_ *= scaleMat;
	worldMat_ *= rotMat;
	worldMat_ *= transMat;
}

void Transform::BillBoardUpdate()
{
	if (isUseBillboard == false)
	{
		billboardMat = Mat4::Identity();
	}
	else
	{
		switch (billboardType)
		{
		case XAxisBillboard:
		{
			billboardMat = Mat4::Identity();
			billboardMat *= ConvertBillBoardXAxis();
			break;
		}
		case YAxisBillboard:
		{
			billboardMat = Mat4::Identity();
			billboardMat *= ConvertBillBoardYAxis();
			break;
		}
		case ZAxisBillboard:
		{
			billboardMat = Mat4::Identity();
			billboardMat *= ConvertBillBoardZAxis();
			break;
		}
		case AllAxisBillboard:
		{
			billboardMat = Mat4::Identity();
			billboardMat *= ConvertBillBoardAllAxis();
			break;
		}
		default:
			break;
		}
	}
}

#pragma region セッター

Mat4 Transform::GetTransMat() { return transMat; }	// 移動行列
Mat4 Transform::GetScaleMat() { return scaleMat; }	// スケール行列
Mat4 Transform::GetRotMat() { return rotMat; }		// 回転行列
Mat4 Transform::GetWorldMat() { return worldMat_; }	// ワールド行列

#pragma endregion

#pragma region セッター

void Transform::SetWorldMat(Mat4 worldMat) { worldMat_ = worldMat; }

#pragma endregion

