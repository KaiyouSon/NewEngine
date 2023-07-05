#pragma once
#include "Mat4.h"
#include "Enum.h"

class Billboard
{
private:
	Mat4 mat_; // ビルボード行列
	BillboardType type_ = BillboardType::None;

public:
	void CalculateBillboardMat();

public: //セッター
	inline void SetBillboardType(const BillboardType type) { type_ = type; }

public:	// ゲッター
	inline Mat4 GetMat() { return mat_; }
	inline BillboardType GetBillboardType() {return type_; }
};

