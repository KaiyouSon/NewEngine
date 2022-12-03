#include "Transform.h"
#include "ViewProjection.h"

Transform::Transform() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	worldMat(Mat4::Identity()), scaleMat(Mat4::Identity()),
	rotMat(Mat4::Identity()), transMat(Mat4::Identity()),
	billboardMat(Mat4::Identity()), isUseBillboard(false), billboardType(XAxisBillboard)
{
}

Transform::Transform(Vec3 pos, Vec3 scale, Vec3 rot) :
	pos(pos), scale(scale), rot(rot),
	worldMat(Mat4::Identity()), scaleMat(Mat4::Identity()),
	rotMat(Mat4::Identity()), transMat(Mat4::Identity()),
	billboardMat(Mat4::Identity()), isUseBillboard(false), billboardType(XAxisBillboard)
{
}

void Transform::Update()
{
	scaleMat = mathUtil->ConvertScalingMat(scale);		 // ƒXƒP[ƒŠƒ“ƒO
	rotMat = Mat4::Identity();
	rotMat *= mathUtil->ConvertRotationZAxisMat(rot.z); // zŽ²‰ñ“]
	rotMat *= mathUtil->ConvertRotationXAxisMat(rot.x); // xŽ²‰ñ“]
	rotMat *= mathUtil->ConvertRotationYAxisMat(rot.y); // yŽ²‰ñ“]
	transMat = mathUtil->ConvertTranslationMat(pos);	 // •½sˆÚ“®

	BillBoardUpdate();

	worldMat = Mat4::Identity();
	worldMat *= billboardMat;
	worldMat *= scaleMat;
	worldMat *= rotMat;
	worldMat *= transMat;
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
			billboardMat *= mathUtil->ConvertBillBoardXAxis();
			break;
		}
		case YAxisBillboard:
		{
			billboardMat = Mat4::Identity();
			billboardMat *= mathUtil->ConvertBillBoardYAxis();
			break;
		}
		case ZAxisBillboard:
		{
			billboardMat = Mat4::Identity();
			billboardMat *= mathUtil->ConvertBillBoardZAxis();
			break;
		}
		case AllAxisBillboard:
		{
			billboardMat = Mat4::Identity();
			billboardMat *= mathUtil->ConvertBillBoardAllAxis();
			break;
		}
		default:
			break;
		}
	}
}
