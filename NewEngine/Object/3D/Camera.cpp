#include "Camera.h"
#include "Util.h"
#include "NewEngine.h"

Camera Camera::current = {};

Camera::Camera() : pos(0, 0, 0), rot(0, 0, 0), fov(Radian(45)), nearZ(0.1f), farZ(1000.0f)
{
	// ��]�֘A
	Transform transform;
	transform.rot = rot;
	transform.Update();
	Vec3 v1 = pos;
	Vec3 v2 = transform.GetWorldMat().ExtractAxisZ();
	Vec3 v3 = transform.GetWorldMat().ExtractAxisY();

	// �r���[�ϊ��s��
	viewProjectionMat = ConvertViewProjectionMatLookTo(v1, v2, v3);

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
	Vec3 v2 = transform.GetWorldMat().ExtractAxisZ();
	Vec3 v3 = transform.GetWorldMat().ExtractAxisY();

	// �r���[�ϊ��s��
	viewProjectionMat = ConvertViewProjectionMatLookTo(v1, v2, v3);

	// ���s���e�s��̌v�Z
	orthoGrphicProjectionMat = ConvertOrthoGrphicProjectionMat(GetWindowSize().x, GetWindowSize().y);

	// �������e�s��̌v�Z
	perspectiveProjectionMat = ConvertPerspectiveProjectionMat(
		fov, (float)GetWindowSize().x / GetWindowSize().y, nearZ, farZ);
}