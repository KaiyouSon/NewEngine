#pragma once
#include "Util.h"
#include "GameObject.h"

// カメラのクラス
class Camera : public GameObject
{
public:
	CameraType cameraType;
	Vec2 oNearFarClip;
	Vec2 pNearFarClip;
	float fovAngle;


	float oNearZ;
	float oFarZ;
	float pNearZ;
	float pFarZ;

	RectAngle rect;
	Frustum viewFrustum;
	static Camera current;

private:
	Mat4 mViewLookToMat;              // ビュー行列 - LookTo 方式
	Mat4 mViewLookAtMat;              // ビュー行列 - LookAt 方式
	Mat4 mOrthographicProjectionMat;  // 正射影行列
	Mat4 mPerspectiveProjectionMat;   // 透視投影行列

	Mat4 mViewMat;
	Mat4 mProjectionMat;

private:
	// コンポーネント関連
	CameraInfo* mInfo;

private:
	void CalcViewFrustum();	// 試錐台を計算

public:
	Camera();
	Camera(const std::string& name);
	void Update() override;
	static void DebugCameraUpdate();

	bool IsVisible(const Vec3 objPos, const float radius);
	Mat4 GetViewLookToMat();
	Mat4 GetViewLookAtMat();
	Mat4 GetOrthoGrphicProjectionMat();
	Mat4 GetPerspectiveProjectionMat();

public:
	void Copy(const Camera& camera);

private:
	void Copy([[maybe_unused]] GameObject* gameObj) override {};
	void ExecuteCS() override {}
	void AppedToRenderer() override {}
	void Draw(const std::string& _layerTag = "", const BlendMode _blendMode = BlendMode::Alpha) override
	{
		_layerTag; _blendMode;
	}
	void SetTexture(const std::string& textureTag, [[maybe_unused]] const bool isChangeSize = true) override { textureTag; };
};