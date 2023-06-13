#pragma once
#include "Mat4.h"
#include "Enum.h"

class Billboard
{
private:
	Mat4 mat_; // �r���{�[�h�s��
	BillboardType type_ = BillboardType::None;

public:
	void CalculateBillboardMat();

public: //�Z�b�^�[
	inline void SetBillboardType(const BillboardType type) { type_ = type; }

public:	// �Q�b�^�[
	inline Mat4 GetMat() { return mat_; }
	inline BillboardType GetBillboardType() {return type_; }
};

