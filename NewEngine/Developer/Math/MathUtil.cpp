#include "MathUtil.h"
#include "ViewProjection.h"

Mat4 MathUtil::ConvertScalingMat(Vec3 scale)
{
	return
	{
		scale.x,0,0,0,
		0,scale.y,0,0,
		0,0,scale.z,0,
		0,0,0,1
	};
}
Mat4 MathUtil::ConvertRotationXAxisMat(float angle)
{
	return
	{
		1,0,0,0,
		0, cosf(MathUtil::Radian(angle)),sinf(MathUtil::Radian(angle)),0,
		0,-sinf(MathUtil::Radian(angle)),cosf(MathUtil::Radian(angle)),0,
		0,0,0,1
	};
}
Mat4 MathUtil::ConvertRotationYAxisMat(float angle)
{
	return
	{
		cosf(MathUtil::Radian(angle)),0,-sinf(MathUtil::Radian(angle)),0,
		0,1,0,0,
		sinf(MathUtil::Radian(angle)),0, cosf(MathUtil::Radian(angle)),0,
		0,0,0,1
	};
}
Mat4 MathUtil::ConvertRotationZAxisMat(float angle)
{
	return
	{
		 cosf(MathUtil::Radian(angle)),sinf(MathUtil::Radian(angle)),0,0,
		-sinf(MathUtil::Radian(angle)),cosf(MathUtil::Radian(angle)),0,0,
		0,0,1,0,
		0,0,0,1
	};
}
Mat4 MathUtil::ConvertTranslationMat(const Vec3& pos)
{
	return
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		pos.x,pos.y,pos.z,1
	};
}

Mat4 MathUtil::ConvertBillBoardXAxis()
{
	Mat4 tempMat = Mat4::Inverse(view->matView);

	tempMat.m[0][0] = 1;
	tempMat.m[0][1] = 0;
	tempMat.m[0][2] = 0;
	tempMat.m[0][3] = 0;

	tempMat.m[3][0] = 0;
	tempMat.m[3][1] = 0;
	tempMat.m[3][2] = 0;
	tempMat.m[3][3] = 1;

	return tempMat;
}
Mat4 MathUtil::ConvertBillBoardYAxis()
{
	Mat4 tempMat = Mat4::Inverse(view->matView);

	tempMat.m[1][0] = 0;
	tempMat.m[1][1] = 1;
	tempMat.m[1][2] = 0;
	tempMat.m[1][3] = 0;

	tempMat.m[3][0] = 0;
	tempMat.m[3][1] = 0;
	tempMat.m[3][2] = 0;
	tempMat.m[3][3] = 1;

	return tempMat;
}
Mat4 MathUtil::ConvertBillBoardZAxis()
{
	Mat4 tempMat = Mat4::Inverse(view->matView);

	tempMat.m[2][0] = 0;
	tempMat.m[2][1] = 0;
	tempMat.m[2][2] = 1;
	tempMat.m[2][3] = 0;

	tempMat.m[3][0] = 0;
	tempMat.m[3][1] = 0;
	tempMat.m[3][2] = 0;
	tempMat.m[3][3] = 1;

	return tempMat;
}
Mat4 MathUtil::ConvertBillBoardAllAxis()
{
	Mat4 tempMat = Mat4::Inverse(view->matView);

	tempMat.m[3][0] = 0;
	tempMat.m[3][1] = 0;
	tempMat.m[3][2] = 0;
	tempMat.m[3][3] = 1;

	return tempMat;
}

Mat4 MathUtil::ConvertViewProjectionMat(const Vec3& pos, const Vec3& target, const Vec3& up)
{
	// 単位行列で初期化
	Mat4 view = Mat4::Identity();

	// ｚ軸
	Vec3 zAxis = target - pos;
	view.m[2][0] = zAxis.Normalized().x;
	view.m[2][1] = zAxis.Normalized().y;
	view.m[2][2] = zAxis.Normalized().z;

	// ｘ軸
	Vec3 xAxis = Vec3::Cross(up, zAxis);
	view.m[0][0] = xAxis.Normalized().x;
	view.m[0][1] = xAxis.Normalized().y;
	view.m[0][2] = xAxis.Normalized().z;

	// ｙ軸
	Vec3 yAxis = Vec3::Cross(zAxis, xAxis);
	view.m[1][0] = yAxis.Normalized().x;
	view.m[1][1] = yAxis.Normalized().y;
	view.m[1][2] = yAxis.Normalized().z;

	// 平行移動
	view.m[3][0] = Vec3::Dot(pos, xAxis.Normalized());
	view.m[3][1] = Vec3::Dot(pos, yAxis.Normalized());
	view.m[3][2] = -Vec3::Dot(pos, zAxis.Normalized());

	return view;
}
Mat4 MathUtil::ConvertPerspectiveProjectionMat(float fovAngle, float aspect, float nearZ, float farZ)
{
	// 単位行列で初期化
	Mat4 perspective = Mat4::Zero();

	float scaleY = 1 / tanf(fovAngle / 2);
	float scaleX = 1 / tanf(fovAngle / 2) / aspect;
	float scaleZ = 1 / (farZ - nearZ) * farZ;
	float TransZ = -nearZ / (farZ - nearZ) * farZ;

	perspective.m[1][1] = scaleY;
	perspective.m[0][0] = scaleX;
	perspective.m[2][2] = scaleZ;
	perspective.m[3][2] = TransZ;
	perspective.m[2][3] = 1;

	return perspective;
}
Mat4 MathUtil::ConvertOrthoGrphicProjectionMat(int WIN_WIDTH, int WIN_HEIGHT)
{
	return
	{
		2 / (float)WIN_WIDTH,0,0,0,
		0,-2 / (float)WIN_HEIGHT,0,0,
		0,0,1,0,
		-1,1,0,1
	};
}

Mat4 MathUtil::ConvertViewportMat(Viewport& viewport)
{
	Mat4 viewportMat = Mat4::Identity();
	viewportMat.m[0][0] = viewport.GetSize().x / 2;
	viewportMat.m[1][1] = -viewport.GetSize().y / 2;
	viewportMat.m[2][2] = viewport.GetMaxDepth() - viewport.GetMinDepth();
	viewportMat.m[3][0] = viewport.GetLeftTopPos().x + viewport.GetSize().x / 2;
	viewportMat.m[3][1] = viewport.GetLeftTopPos().y + viewport.GetSize().y / 2;
	viewportMat.m[3][2] = viewport.GetMinDepth();
	return viewportMat;
}

MathUtil* mathUtil = MathUtil::GetInstance().get();