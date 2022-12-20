#pragma once
#include "Util.h"

class Camera
{
public:
	Vec3 pos = { 0,0,-30 };		// ���_���W
	Vec3 rot = { 0,0,0 };
	float nearZ;
	float farZ;
	float fov;
	static Camera current;

private:
	Mat4 viewLookToMat;			// �r���[�ϊ��s��i LookTo �j
	Mat4 viewLookAtMat;			// �r���[�ϊ��s��i LookAt �j
	Mat4 orthoGrphicProjectionMat;	// ���s���e�s��
	Mat4 perspectiveProjectionMat;	// �������e�s��
	
public:
	Camera();
	void Update();
	static void DebugCameraUpdate();

	inline Mat4 GetViewLookToMat() { return viewLookToMat; }
	inline Mat4 GetViewLookAtMat() { return viewLookAtMat; }
	inline Mat4 GetOrthoGrphicProjectionMat() { return orthoGrphicProjectionMat; }
	inline Mat4 GetPerspectiveProjectionMat() { return perspectiveProjectionMat; }
};
