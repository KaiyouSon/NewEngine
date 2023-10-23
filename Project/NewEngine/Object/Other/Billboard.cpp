#include "Billboard.h"
#include "MathUtil.h"

Billboard::Billboard() :
	mMat(Mat4::Identity()), mType(BillboardType::None)
{
}

void Billboard::CalculateBillboardMat()
{
	mMat = Mat4::Identity();

	switch (mType)
	{
	case BillboardType::XAxisBillboard:	// X軸ビルボード
	{
		mMat *= ConvertBillBoardXAxis();
		break;
	}

	case BillboardType::YAxisBillboard:	// Y軸ビルボード
	{
		mMat *= ConvertBillBoardYAxis();
		break;
	}

	case BillboardType::ZAxisBillboard:	// Z軸ビルボード
	{
		mMat *= ConvertBillBoardZAxis();
		break;
	}

	case BillboardType::AllAxisBillboard:	// 全方位ビルボード
	{
		mMat *= ConvertBillBoardAllAxis();
		break;
	}

	default:
		break;

	}
}

// セッター
void Billboard::SetBillboardType(const BillboardType type) { mType = type; }

// ゲッター
Mat4 Billboard::GetMat() { return mMat; }
BillboardType Billboard::GetBillboardType() { return mType; }