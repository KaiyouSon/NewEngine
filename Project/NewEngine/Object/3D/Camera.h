#pragma once
#include "Util.h"

class Camera
{
public:
	Vec3 pos;
	Vec3 rot;
	float oNearZ;
	float oFarZ;
	float pNearZ;
	float pFarZ;
	float fov;
	RectAngle rect;
	static Camera current;

private:
	Mat4 mViewLookToMat;			// �r���[�ϊ��s��i LookTo �j
	Mat4 mViewLookAtMat;			// �r���[�ϊ��s��i LookAt �j
	Mat4 mOrthoGrphicProjectionMat;	// ���s���e�s��
	Mat4 mPerspectiveProjectionMat;	// �������e�s��

public:
	Camera();
	void Update();
	static void DebugCameraUpdate();

	inline Mat4 GetViewLookToMat() { return mViewLookToMat; }
	inline Mat4 GetViewLookAtMat() { return mViewLookAtMat; }
	inline Mat4 GetOrthoGrphicProjectionMat() { return mOrthoGrphicProjectionMat; }
	inline Mat4 GetPerspectiveProjectionMat() { return mPerspectiveProjectionMat; }
};
