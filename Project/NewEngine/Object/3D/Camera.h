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
	Mat4 mViewLookToMat;			// ビュー変換行列（ LookTo ）
	Mat4 mViewLookAtMat;			// ビュー変換行列（ LookAt ）
	Mat4 mOrthoGrphicProjectionMat;	// 平行投影行列
	Mat4 mPerspectiveProjectionMat;	// 透視投影行列

public:
	Camera();
	void Update();
	static void DebugCameraUpdate();

	inline Mat4 GetViewLookToMat() { return mViewLookToMat; }
	inline Mat4 GetViewLookAtMat() { return mViewLookAtMat; }
	inline Mat4 GetOrthoGrphicProjectionMat() { return mOrthoGrphicProjectionMat; }
	inline Mat4 GetPerspectiveProjectionMat() { return mPerspectiveProjectionMat; }
};
