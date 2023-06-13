#include "Billboard.h"
#include "MathUtil.h"

void Billboard::CalculateBillboardMat()
{
	mat_ = Mat4::Identity();

	switch (type_)
	{
	case BillboardType::XAxisBillboard:	// X軸ビルボード
	{
		mat_ *= ConvertBillBoardXAxis();
		break;
	}

	case BillboardType::YAxisBillboard:	// Y軸ビルボード
	{
		mat_ *= ConvertBillBoardYAxis();
		break;
	}

	case BillboardType::ZAxisBillboard:	// Z軸ビルボード
	{
		mat_ *= ConvertBillBoardZAxis();
		break;
	}

	case BillboardType::AllAxisBillboard:	// 全方位ビルボード
	{
		mat_ *= ConvertBillBoardAllAxis();
		break;
	}

	default:
		break;

	}
}
