#include "Transform.h"
#include "MathUtil.h"

Transform::Transform() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	worldMat_(Mat4::Identity()), scaleMat_(Mat4::Identity()),
	rotMat_(Mat4::Identity()), transMat_(Mat4::Identity()),
	billboardMat_(Mat4::Identity()), isUseBillboard_(false),
	billboardType_(BillBoardType::XAxisBillboard)
{
}

Transform::Transform(const Vec3 pos, const Vec3 scale, const Vec3 rot) :
	pos(pos), scale(scale), rot(rot),
	worldMat_(Mat4::Identity()), scaleMat_(Mat4::Identity()),
	rotMat_(Mat4::Identity()), transMat_(Mat4::Identity()),
	billboardMat_(Mat4::Identity()), isUseBillboard_(false),
	billboardType_(BillBoardType::XAxisBillboard)
{
}

void Transform::Update()
{
	scaleMat_ = ConvertScalingMat(scale);		 // �X�P�[�����O
	rotMat_ = Mat4::Identity();
	rotMat_ *= ConvertRotationZAxisMat(rot.z); // z����]
	rotMat_ *= ConvertRotationXAxisMat(rot.x); // x����]
	rotMat_ *= ConvertRotationYAxisMat(rot.y); // y����]
	transMat_ = ConvertTranslationMat(pos);	 // ���s�ړ�

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
		case BillBoardType::XAxisBillboard:
		{
			billboardMat_ = Mat4::Identity();
			billboardMat_ *= ConvertBillBoardXAxis();
			break;
		}
		case BillBoardType::YAxisBillboard:
		{
			billboardMat_ = Mat4::Identity();
			billboardMat_ *= ConvertBillBoardYAxis();
			break;
		}
		case BillBoardType::ZAxisBillboard:
		{
			billboardMat_ = Mat4::Identity();
			billboardMat_ *= ConvertBillBoardZAxis();
			break;
		}
		case BillBoardType::AllAxisBillboard:
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

#pragma region �Z�b�^�[

Mat4 Transform::GetTransMat() { return transMat_; }	// �ړ��s��
Mat4 Transform::GetScaleMat() { return scaleMat_; }	// �X�P�[���s��
Mat4 Transform::GetRotMat() { return rotMat_; }		// ��]�s��
Mat4 Transform::GetWorldMat() { return worldMat_; }	// ���[���h�s��

#pragma endregion

#pragma region �Z�b�^�[

void Transform::SetWorldMat(Mat4 worldMat) { worldMat_ = worldMat; }

#pragma endregion

