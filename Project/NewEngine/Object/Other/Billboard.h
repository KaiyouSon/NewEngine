#pragma once
#include "Mat4.h"
#include "Enum.h"

class Billboard
{
private:
	Mat4 mat_; // 繝薙Ν繝懊・繝芽｡悟・
	BillboardType type_ = BillboardType::None;

public:
	void CalculateBillboardMat();

public: //繧ｻ繝・ち繝ｼ
	inline void SetBillboardType(const BillboardType type) { type_ = type; }

public:	// 繧ｲ繝・ち繝ｼ
	inline Mat4 GetMat() { return mat_; }
	inline BillboardType GetBillboardType() {return type_; }
};
