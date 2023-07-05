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
	Mat4 viewLookToMat_;			// �r���[�ϊ��s��i LookTo �j
	Mat4 viewLookAtMat_;			// �r���[�ϊ��s��i LookAt �j
	Mat4 orthoGrphicProjectionMat_;	// ���s���e�s��
	Mat4 perspectiveProjectionMat_;	// �������e�s��
	
public:
	Camera();
	void Update();
	static void DebugCameraUpdate();

	inline Mat4 GetViewLookToMat() { return viewLookToMat_; }
	inline Mat4 GetViewLookAtMat() { return viewLookAtMat_; }
	inline Mat4 GetOrthoGrphicProjectionMat() { return orthoGrphicProjectionMat_; }
	inline Mat4 GetPerspectiveProjectionMat() { return perspectiveProjectionMat_; }
};
