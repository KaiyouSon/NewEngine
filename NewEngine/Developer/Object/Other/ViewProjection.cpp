#include "ViewProjection.h"
#include "Util.h"
#include "MathUtil.h"

View* view = View::GetInstance().get();

void ViewProjection::Initialize()
{
	// �r���[�ϊ��s��
	matView = mathUtil->ConvertViewProjectionMat(pos, target, up);

	// ���s���e�s��̌v�Z
	matProjection2D = mathUtil->ConvertOrthoGrphicProjectionMat(WIN_WIDTH, WIN_HEIGHT);

	nearZ = 0.1f, farZ = 1000.0f;

	// �������e�s��̌v�Z
	matProjection3D = mathUtil->ConvertPerspectiveProjectionMat(
		mathUtil->Radian(45),	// �㉺��p45�x
		(float)WIN_WIDTH / WIN_HEIGHT, // �A�X�y�N�g��(��ʉ���/��ʏc��)
		nearZ, farZ);	// ��[�@���[
}

void ViewProjection::Update()
{
	matView = mathUtil->ConvertViewProjectionMat(pos, target, up);
}