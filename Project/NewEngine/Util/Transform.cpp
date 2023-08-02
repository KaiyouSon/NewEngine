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
	mScaleMat = ConvertScalingMat(scale); // �X�P�[�����O
	mRotMat = Mat4::Identity();
	mRotMat *= ConvertRotationZAxisMat(rot.z); // z����]
	mRotMat *= ConvertRotationXAxisMat(rot.x); // x����]
	mRotMat *= ConvertRotationYAxisMat(rot.y); // y����]
	mTransMat = ConvertTranslationMat(pos);	 // ���s�ړ�

	// �r���{�[�h�s��v�Z
	mBillboard.CalculateBillboardMat();

	// ���[���h�s��v�Z
	mWorldMat = Mat4::Identity();

	if (mBillboard.GetBillboardType() != BillboardType::None)
	{
		mWorldMat *= mBillboard.GetMat();
	}

	mWorldMat *= mScaleMat;
	mWorldMat *= mRotMat;
	mWorldMat *= mTransMat;
}

#pragma region �Z�b�^�[

Mat4 Transform::GetTransMat() { return mTransMat; }	// �ړ��s��
Mat4 Transform::GetScaleMat() { return mScaleMat; }	// �X�P�[���s��
Mat4 Transform::GetRotMat() { return mRotMat; }		// ��]�s��
Mat4 Transform::GetWorldMat() { return mWorldMat; }	// ���[���h�s��

#pragma endregion

#pragma region �Z�b�^�[

void Transform::SetWorldMat(Mat4 worldMat) { mWorldMat = worldMat; }

void Transform::SetBillboardType(const BillboardType type) { mBillboard.SetBillboardType(type); }

#pragma endregion

