#include "Camera.h"
#include "Util.h"
#include "NewEngine.h"

Camera Camera::current = {};

Camera::Camera() : pos(0, 0, 0), rot(0, 0, 0), fov(Radian(45)), nearZ(0.1f), farZ(1000.0f)
{
	// 回転関連
	Transform transform;
	transform.rot = rot;
	transform.Update();
	Vec3 v1 = pos;
	Vec3 v2 = transform.GetWorldMat().ExtractAxisZ();
	Vec3 v3 = transform.GetWorldMat().ExtractAxisY();

	// ビュー変換行列
	viewProjectionMat = ConvertViewProjectionMatLookTo(v1, v2, v3);

	// 並行投影行列の計算
	orthoGrphicProjectionMat = ConvertOrthoGrphicProjectionMat(GetWindowSize().x, GetWindowSize().y);

	// 透視投影行列の計算
	perspectiveProjectionMat = ConvertPerspectiveProjectionMat(
		fov, (float)GetWindowSize().x / GetWindowSize().y, nearZ, farZ);
}

void Camera::Update()
{
	// 回転関連
	Transform transform;
	transform.rot = rot;
	transform.Update();
	Vec3 v1 = pos;
	Vec3 v2 = transform.GetWorldMat().ExtractAxisZ();
	Vec3 v3 = transform.GetWorldMat().ExtractAxisY();

	// ビュー変換行列
	viewProjectionMat = ConvertViewProjectionMatLookTo(v1, v2, v3);

	// 平行投影行列の計算
	orthoGrphicProjectionMat = ConvertOrthoGrphicProjectionMat(GetWindowSize().x, GetWindowSize().y);

	// 透視投影行列の計算
	perspectiveProjectionMat = ConvertPerspectiveProjectionMat(
		fov, (float)GetWindowSize().x / GetWindowSize().y, nearZ, farZ);
}