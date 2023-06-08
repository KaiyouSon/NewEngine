#pragma once
#include "Vec3.h"
#include "Mat4.h"
#include "Enum.h"

class Transform
{
public:
	Vec3 pos;	// ���W
	Vec3 scale;	// �X�P�[��
	Vec3 rot;	// ��]

private:
	Mat4 scaleMat_;	// �X�P�[���s��
	Mat4 rotMat_;	// ��]�s��
	Mat4 transMat_;	// ���s�ړ��s��
	Mat4 worldMat_;	// ���[���h�ϊ��s��
	Mat4 billboardMat_; // �r���{�[�h�s��

	bool isUseBillboard_;
	BillBoardType billboardType_;

private:
	void BillBoardUpdate();

public:
	Transform();
	Transform(const Vec3 pos, const Vec3 scale, const Vec3 rot);
	void Update();

public:	// �Z�b�^�[
	inline void SetisUseBillboard(const bool isUseBillboard) { isUseBillboard_ = isUseBillboard; }
	inline void SetBillboardType(const BillBoardType billboardType) { billboardType_ = billboardType; }

	inline bool GetisUseBillboard() { return isUseBillboard_; }
	inline BillBoardType GetBillboardType() { return billboardType_; }

#pragma region �Q�b�^�[

	Mat4 GetTransMat();
	Mat4 GetScaleMat();
	Mat4 GetRotMat();
	Mat4 GetWorldMat();

#pragma endregion

#pragma region �Z�b�^�[
	void SetWorldMat(Mat4 worldMat);

#pragma endregion
};

