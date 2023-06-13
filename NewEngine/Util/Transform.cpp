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
	scaleMat_ = ConvertScalingMat(scale);		 // �X�P�[�����O
	rotMat_ = Mat4::Identity();
	rotMat_ *= ConvertRotationZAxisMat(rot.z); // z����]
	rotMat_ *= ConvertRotationXAxisMat(rot.x); // x����]
	rotMat_ *= ConvertRotationYAxisMat(rot.y); // y����]
	transMat_ = ConvertTranslationMat(pos);	 // ���s�ړ�

	// �r���{�[�h�s��v�Z
	billboard_.CalculateBillboardMat();

	// ���[���h�s��v�Z
	worldMat_ = Mat4::Identity();

	if (billboard_.GetBillboardType() != BillboardType::None)
	{
		worldMat_ *= billboard_.GetMat();
	}

	worldMat_ *= scaleMat_;
	worldMat_ *= rotMat_;
	worldMat_ *= transMat_;
}

#pragma region �Z�b�^�[

Mat4 Transform::GetTransMat() { return transMat_; }	// �ړ��s��
Mat4 Transform::GetScaleMat() { return scaleMat_; }	// �X�P�[���s��
Mat4 Transform::GetRotMat() { return rotMat_; }		// ��]�s��
Mat4 Transform::GetWorldMat() { return worldMat_; }	// ���[���h�s��

#pragma endregion

#pragma region �Z�b�^�[

void Transform::SetWorldMat(Mat4 worldMat) { worldMat_ = worldMat; }

void Transform::SetBillboardType(const BillboardType type) { billboard_.SetBillboardType(type); }

#pragma endregion

