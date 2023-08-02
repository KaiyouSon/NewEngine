#pragma once
#include "Vec3.h"
#include "Billboard.h"

class Transform
{
private:
	Mat4 mScaleMat;	// �X�P�[���s��
	Mat4 mRotMat;	// ��]�s��
	Mat4 mTransMat;	// ���s�ړ��s��
	Mat4 mWorldMat;	// ���[���h�ϊ��s��

	Billboard mBillboard;

public:
	Vec3 pos;	// ���W
	Vec3 scale;	// �X�P�[��
	Vec3 rot;	// ��]

public:
	Transform();
	Transform(const Vec3 pos, const Vec3 scale, const Vec3 rot);
	void Update();

public: // �Q�b�^�[
	Mat4 GetTransMat();
	Mat4 GetScaleMat();
	Mat4 GetRotMat();
	Mat4 GetWorldMat();

public: // �Z�b�^�[
	void SetWorldMat(Mat4 worldMat);
	void SetBillboardType(const BillboardType type);
};

