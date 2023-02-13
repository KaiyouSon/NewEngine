#include "Transform.h"
#include "MathUtil.h"

Transform::Transform() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	worldMat(Mat4::Identity()), scaleMat(Mat4::Identity()),
	rotMat(Mat4::Identity()), transMat(Mat4::Identity()),
	billboardMat(Mat4::Identity()), isUseBillboard(false), billboardType(XAxisBillboard)
{
}

Transform::Transform(const Vec3& pos, const Vec3& scale, const Vec3& rot) :
	pos(pos), scale(scale), rot(rot),
	worldMat(Mat4::Identity()), scaleMat(Mat4::Identity()),
	rotMat(Mat4::Identity()), transMat(Mat4::Identity()),
	billboardMat(Mat4::Identity()), isUseBillboard(false), billboardType(XAxisBillboard)
{
}

void Transform::Update()
{
	scaleMat = ConvertScalingMat(scale);		 // �X�P�[�����O
	rotMat = Mat4::Identity();
	rotMat *= ConvertRotationZAxisMat(rot.z); // z����]
	rotMat *= ConvertRotationXAxisMat(rot.x); // x����]
	rotMat *= ConvertRotationYAxisMat(rot.y); // y����]
	transMat = ConvertTranslationMat(pos);	 // ���s�ړ�

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
