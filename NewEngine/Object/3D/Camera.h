#pragma once
#include "Util.h"

class Camera
{
public:
	Vec3 pos = { 0,0,-30 };		// 視点座標
	Vec3 rot = { 0,0,0 };
	float nearZ;
	float farZ;
	float fov;
	static Camera current;

private:
	Mat4 viewLookToMat_;			// ビュー変換行列（ LookTo ）
	Mat4 viewLookAtMat_;			// ビュー変換行列（ LookAt ）
	Mat4 orthoGrphicProjectionMat_;	// 平行投影行列
	Mat4 perspectiveProjectionMat_;	// 透視投影行列
	
public:
	Camera();
	void Update();
	static void DebugCameraUpdate();

	inline Mat4 GetViewLookToMat() { return viewLookToMat_; }
	inline Mat4 GetViewLookAtMat() { return viewLookAtMat_; }
	inline Mat4 GetOrthoGrphicProjectionMat() { return orthoGrphicProjectionMat_; }
	inline Mat4 GetPerspectiveProjectionMat() { return perspectiveProjectionMat_; }
};
