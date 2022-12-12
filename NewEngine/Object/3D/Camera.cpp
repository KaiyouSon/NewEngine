#include "Camera.h"
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

void Camera::DebugCameraUpdate()
{
	const Vec3 frontVec =
	{
		sinf(current.rot.y),
		-sinf(current.rot.x),
		cosf(current.rot.y),
	};

	// 回転
	if (Mouse::GetClick(MouseCodo::Wheel) && !Key::GetKey(DIK_LSHIFT))
	{
		if (Mouse::GetMouseMove().x != 0 || Mouse::GetMouseMove().y != 0)
		{
			const float moveSpeed = 0.005f;
			current.rot.x += Mouse::GetMouseMove().y * moveSpeed;
			current.rot.y += Mouse::GetMouseMove().x * moveSpeed;
		}
	}

	// 平行移動
	if (Mouse::GetClick(MouseCodo::Wheel) && Key::GetKey(DIK_LSHIFT))
	{
		if (Mouse::GetMouseMove().x != 0 || Mouse::GetMouseMove().y != 0)
		{
			const Vec3 rightVec = Vec3::Cross(frontVec, { 0,1,0 });
			current.pos += rightVec * Mouse::GetMouseMove().x * 0.1f;
		}
	}

	// 奥行
	if (Mouse::GetWheelMove() != 0)
	{
		const float moveSpeed = 0.025f;
		current.pos += frontVec * Mouse::GetWheelMove() * 0.025f;
	}
}
