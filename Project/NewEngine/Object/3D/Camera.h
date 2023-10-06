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
	Mat4 mViewLookToMat;			// 繝薙Η繝ｼ螟画鋤陦悟・・・LookTo ・・
	Mat4 mViewLookAtMat;			// 繝薙Η繝ｼ螟画鋤陦悟・・・LookAt ・・
	Mat4 mOrthoGrphicProjectionMat;	// 蟷ｳ陦梧兜蠖ｱ陦悟・
	Mat4 mPerspectiveProjectionMat;	// 騾剰ｦ匁兜蠖ｱ陦悟・

public:
	Camera();
	void Update();
	static void DebugCameraUpdate();

	Mat4 GetViewLookToMat();
	Mat4 GetViewLookAtMat();
	Mat4 GetOrthoGrphicProjectionMat();
	Mat4 GetPerspectiveProjectionMat();
};
