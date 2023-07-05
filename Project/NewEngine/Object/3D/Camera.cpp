#include "Camera.h"
#include "NewEngine.h"

Camera Camera::current = {};

Camera::Camera() : pos(0, 0, 0), rot(0, 0, 0), fov(Radian(45)), nearZ(0.1f), farZ(10000.0f)
{
	// ��]�֘A
	Transform transform;
	transform.rot = rot;
	transform.Update();
	Vec3 v1 = pos;
	Vec3 v2 = transform.GetWorldMat().GetZAxis();
	Vec3 v3 = transform.GetWorldMat().GetYAxis();

	// �r���[�ϊ��s��
	viewLookToMat_ = ConvertViewProjectionMatLookTo(v1, v2, v3);
	viewLookAtMat_ = ConvertViewProjectionMatLookAt(v1, v1 * 10, { 0,1,0 });

	// ���s���e�s��̌v�Z
	orthoGrphicProjectionMat_ = ConvertOrthoGrphicProjectionMat(GetWindowSize().x, GetWindowSize().y);

	// �������e�s��̌v�Z
	perspectiveProjectionMat_ = ConvertPerspectiveProjectionMat(
		fov, (float)GetWindowSize().x / GetWindowSize().y, nearZ, farZ);
}

void Camera::Update()
{
	// ��]�֘A
	Transform transform;
	transform.rot = rot;
	transform.Update();
	Vec3 v1 = pos;
	Vec3 v2 = transform.GetWorldMat().GetZAxis();
	Vec3 v3 = transform.GetWorldMat().GetYAxis();

	// �r���[�ϊ��s��
	viewLookToMat_ = ConvertViewProjectionMatLookTo(v1, v2, v3);
	viewLookAtMat_ = ConvertViewProjectionMatLookAt(v1, v1 * 10, { 0,1,0 });

	// ���s���e�s��̌v�Z
	orthoGrphicProjectionMat_ = ConvertOrthoGrphicProjectionMat(GetWindowSize().x, GetWindowSize().y);

	// �������e�s��̌v�Z
	perspectiveProjectionMat_ = ConvertPerspectiveProjectionMat(
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
		if (Mouse::GetMoveVec().x != 0 || Mouse::GetMoveVec().y != 0)
		{
			const float moveSpeed = 0.005f;
			current.rot.x += Mouse::GetMoveVec().y * moveSpeed;
			current.rot.y += Mouse::GetMoveVec().x * moveSpeed;
		}
	}

	// ���s�ړ�
	if (Mouse::GetClick(MouseCodo::Wheel) && Key::GetKey(DIK_LSHIFT))
	{
		if (Mouse::GetMoveVec().x != 0 || Mouse::GetMoveVec().y != 0)
		{
			const Vec3 rightVec = Vec3::Cross(frontVec, { 0,1,0 });
			current.pos += rightVec * Mouse::GetMoveVec().x * 0.1f;
		}
	}

	// ���s
	if (Mouse::GetWheelMoveVec() != 0)
	{
		const float moveSpeed = 0.025f;
		current.pos += frontVec * Mouse::GetWheelMoveVec() * 0.025f;
	}
}