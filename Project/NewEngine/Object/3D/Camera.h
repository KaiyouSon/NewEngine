#pragma once
#include "Util.h"

// カメラのクラス
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
	Frustum viewFrustum;
	static Camera current;

private:
	Mat4 mViewLookToMat;              // ビュー行列 - LookTo 方式
	Mat4 mViewLookAtMat;              // ビュー行列 - LookAt 方式
	Mat4 mOrthographicProjectionMat;  // 正射影行列
	Mat4 mPerspectiveProjectionMat;   // 透視投影行列

private:
	void CalcViewFrustum();	// 試錐台を計算

public:
	Camera();
	void Update();
	static void DebugCameraUpdate();

	bool IsVisible(const Vec3 objPos, const float radius);
	Mat4 GetViewLookToMat();
	Mat4 GetViewLookAtMat();
	Mat4 GetOrthoGrphicProjectionMat();
	Mat4 GetPerspectiveProjectionMat();
};
