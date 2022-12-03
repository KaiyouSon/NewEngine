#include "ViewProjection.h"
#include "Util.h"
#include "MathUtil.h"

View* view = View::GetInstance().get();

void ViewProjection::Initialize()
{
	// ビュー変換行列
	matView = mathUtil->ConvertViewProjectionMat(pos, target, up);

	// 並行投影行列の計算
	matProjection2D = mathUtil->ConvertOrthoGrphicProjectionMat(WIN_WIDTH, WIN_HEIGHT);

	nearZ = 0.1f, farZ = 1000.0f;

	// 透視投影行列の計算
	matProjection3D = mathUtil->ConvertPerspectiveProjectionMat(
		mathUtil->Radian(45),	// 上下画角45度
		(float)WIN_WIDTH / WIN_HEIGHT, // アスペクト比(画面横幅/画面縦幅)
		nearZ, farZ);	// 先端　奥端
}

void ViewProjection::Update()
{
	matView = mathUtil->ConvertViewProjectionMat(pos, target, up);
}