#include "Camera.h"
#include "Util.h"
#include "NewEngine.h"

Camera Camera::current = {};

Camera::Camera() : pos(0, 0, 0), rot(0, 0, 0), fov(mathUtil->Radian(45)), nearZ(0.1f), farZ(1000.0f)
{
	// ‰ñ“]ŠÖ˜A
	Transform transform;
	transform.rot = rot;
	transform.Update();
	Vec3 v1 = pos;
	Vec3 v2 = transform.GetWorldMat().ExtractAxisZ();
	Vec3 v3 = transform.GetWorldMat().ExtractAxisY();

	// ƒrƒ…[•ÏŠ·s—ñ
	viewProjectionMat = mathUtil->ConvertViewProjectionMatLookTo(v1, v2, v3);

	// •Às“Š‰es—ñ‚ÌŒvŽZ
	orthoGrphicProjectionMat = mathUtil->ConvertOrthoGrphicProjectionMat(GetWindowSize().x, GetWindowSize().y);

	// “§Ž‹“Š‰es—ñ‚ÌŒvŽZ
	perspectiveProjectionMat = mathUtil->ConvertPerspectiveProjectionMat(
		fov, (float)GetWindowSize().x / GetWindowSize().y, nearZ, farZ);
}

void Camera::Update()
{
	// ‰ñ“]ŠÖ˜A
	Transform transform;
	transform.rot = rot;
	transform.Update();
	Vec3 v1 = pos;
	Vec3 v2 = transform.GetWorldMat().ExtractAxisZ();
	Vec3 v3 = transform.GetWorldMat().ExtractAxisY();

	// ƒrƒ…[•ÏŠ·s—ñ
	viewProjectionMat = mathUtil->ConvertViewProjectionMatLookTo(v1, v2, v3);

	// •½s“Š‰es—ñ‚ÌŒvŽZ
	orthoGrphicProjectionMat = mathUtil->ConvertOrthoGrphicProjectionMat(GetWindowSize().x, GetWindowSize().y);

	// “§Ž‹“Š‰es—ñ‚ÌŒvŽZ
	perspectiveProjectionMat = mathUtil->ConvertPerspectiveProjectionMat(
		fov, (float)GetWindowSize().x / GetWindowSize().y, nearZ, farZ);
}