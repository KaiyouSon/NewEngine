#pragma once
#include "Mat4.h"
#include "NewEngineEnum.h"

// ビルボードのクラス
class Billboard
{
private:
	Mat4 mMat;
	BillboardType mType;

public:
	Billboard();
	void CalculateBillboardMat();

public:
	void SetBillboardType(const BillboardType type);

public:
	Mat4 GetMat();
	BillboardType GetBillboardType();
};
