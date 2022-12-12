#pragma once
#include "Util.h"

class Camera
{
public:
	Vec3 pos = { 0,0,-30 };		// 視点座標
	Vec3 rot = { 0,0,0 };
	//Vec3 target = { 0,0,0 };	// 注視点座標
	//Vec3 up = { 0,1,0 };		// 上方向ベクトル
	float nearZ;
	float farZ;
	float fov;
	static Camera current;

private:
	Mat4 viewProjectionMat;			// ビュー変換行列
	Mat4 orthoGrphicProjectionMat;	// 平行投影行列
	Mat4 perspectiveProjectionMat;	// 透視投影行列

public:
	Camera();
	void Update();
	static void DebugCameraUpdate();

	inline Mat4 GetViewProjectionMat() { return viewProjectionMat; }
	inline Mat4 GetOrthoGrphicProjectionMat() { return orthoGrphicProjectionMat; }
	inline Mat4 GetPerspectiveProjectionMat() { return perspectiveProjectionMat; }
};
