#include "Billboard.h"
#include "MathUtil.h"

void Billboard::CalculateBillboardMat()
{
	mat_ = Mat4::Identity();

	switch (type_)
	{
	case BillboardType::XAxisBillboard:	// X���r���{�[�h
	{
		mat_ *= ConvertBillBoardXAxis();
		break;
	}

	case BillboardType::YAxisBillboard:	// Y���r���{�[�h
	{
		mat_ *= ConvertBillBoardYAxis();
		break;
	}

	case BillboardType::ZAxisBillboard:	// Z���r���{�[�h
	{
		mat_ *= ConvertBillBoardZAxis();
		break;
	}

	case BillboardType::AllAxisBillboard:	// �S���ʃr���{�[�h
	{
		mat_ *= ConvertBillBoardAllAxis();
		break;
	}

	default:
		break;

	}
}
