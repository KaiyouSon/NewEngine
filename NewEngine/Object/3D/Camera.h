#pragma once
#include "Util.h"

class Camera
{
public:
	Vec3 pos = { 0,0,-30 };		// ���_���W
	Vec3 rot = { 0,0,0 };
	//Vec3 target = { 0,0,0 };	// �����_���W
	//Vec3 up = { 0,1,0 };		// ������x�N�g��
	float nearZ;
	float farZ;
	float fov;
	static Camera current;

private:
	Mat4 viewProjectionMat;			// �r���[�ϊ��s��
	Mat4 orthoGrphicProjectionMat;	// ���s���e�s��
	Mat4 perspectiveProjectionMat;	// �������e�s��

public:
	Camera();
	void Update();
	static void DebugCameraUpdate();

	inline Mat4 GetViewProjectionMat() { return viewProjectionMat; }
	inline Mat4 GetOrthoGrphicProjectionMat() { return orthoGrphicProjectionMat; }
	inline Mat4 GetPerspectiveProjectionMat() { return perspectiveProjectionMat; }
};
