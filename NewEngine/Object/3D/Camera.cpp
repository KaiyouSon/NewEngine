#include "Camera.h"
#include "NewEngine.h"

Camera Camera::current = {};

Camera::Camera() : pos(0, 0, 0), rot(0, 0, 0), fov(Radian(45)), nearZ(0.1f), farZ(1000.0f)
{
	// ��]�֘A
	Transform transform;
	transform.rot = rot;
	transform.Update();
	Vec3 v1 = pos;
	Vec3 v2 = transform.worldMat.GetZAxis();
	Vec3 v3 = transform.worldMat.GetYAxis();

	// �r���[�ϊ��s��
	viewLookToMat = ConvertViewProjectionMatLookTo(v1, v2, v3);
	viewLookAtMat = ConvertViewProjectionMatLookAt(v1, v1 * 10, { 0,1,0 });

	// ���s���e�s��̌v�Z
	orthoGrphicProjectionMat = ConvertOrthoGrphicProjectionMat(GetWindowSize().x, GetWindowSize().y);

	// �������e�s��̌v�Z
	perspectiveProjectionMat = ConvertPerspectiveProjectionMat(
		fov, (float)GetWindowSize().x / GetWindowSize().y, nearZ, farZ);
}

void Camera::Update()
{
	// ��]�֘A
	Transform transform;
	transform.rot = rot;
	transform.Update();
	Vec3 v1 = pos;
	Vec3 v2 = transform.worldMat.GetZAxis();
	Vec3 v3 = transform.worldMat.GetYAxis();

	// �r���[�ϊ��s��
	viewLookToMat = ConvertViewProjectionMatLookTo(v1, v2, v3);
	viewLookAtMat = ConvertViewProjectionMatLookAt(v1, v1 * 10, { 0,1,0 });

	// ���s���e�s��̌v�Z
	orthoGrphicProjectionMat = ConvertOrthoGrphicProjectionMat(GetWindowSize().x, GetWindowSize().y);

	// �������e�s��̌v�Z
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

	// ��]
	if (Mouse::GetClick(MouseCodo::Wheel) && !Key::GetKey(DIK_LSHIFT))
	{
		if (Mouse::GetMouseMove().x != 0 || Mouse::GetMouseMove().y != 0)
		{
			const float moveSpeed = 0.005f;
			current.rot.x += Mouse::GetMouseMove().y * moveSpeed;
			current.rot.y += Mouse::GetMouseMove().x * moveSpeed;
		}
	}

	// ���s�ړ�
	if (Mouse::GetClick(MouseCodo::Wheel) && Key::GetKey(DIK_LSHIFT))
	{
		if (Mouse::GetMouseMove().x != 0 || Mouse::GetMouseMove().y != 0)
		{
			const Vec3 rightVec = Vec3::Cross(frontVec, { 0,1,0 });
			current.pos += rightVec * Mouse::GetMouseMove().x * 0.1f;
		}
	}

	// ���s
	if (Mouse::GetWheelMove() != 0)
	{
		const float moveSpeed = 0.025f;
		current.pos += frontVec * Mouse::GetWheelMove() * 0.025f;
	}
}
