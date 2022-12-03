#pragma once
#include "IComponent.h"
#include "MathUtil.h"

enum BillBoardType
{
	XAxisBillboard,
	YAxisBillboard,
	ZAxisBillboard,
	AllAxisBillboard,
};

class Transform : public IComponent
{
public:
	Vec3 pos;	// ���W
	Vec3 scale;	// �X�P�[��
	Vec3 rot;	// ��]

	// ���[���h�ϊ��s��
	Mat4 worldMat;
private:
	Mat4 scaleMat;	// �X�P�[���s��
	Mat4 rotMat;	// ��]�s��
	Mat4 transMat;	// ���s�ړ��s��
	Mat4 billboardMat;

	bool isUseBillboard;
	int billboardType;

private:
	void BillBoardUpdate();

public:
	Transform();
	Transform(Vec3 pos, Vec3 scale, Vec3 rot);
	void Update();

public:	// �Z�b�^�[
	inline void SetisUseBillboard(const bool& isUseBillboard) { this->isUseBillboard = isUseBillboard; }
	inline void SetBillboardType(const int& billboardType) { this->billboardType = billboardType; }

	inline bool GetisUseBillboard() { return isUseBillboard; }
	inline int GetBillboardType() { return billboardType; }

};

