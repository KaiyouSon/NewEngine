#pragma once
#include "NewEngine.h"
#include "ShadowObj.h"

// 前方宣言
template<typename T> class Singleton;

// シャドウマップ制御するクラス
class ShadowMap : public Singleton<ShadowMap>
{
private:
	std::vector<ShadowObj> mShadowObjs;
	std::vector<Transform> mParents;
	uint32_t mIndex;
	Vec2 rectLeftTop;
	Vec2 rectSize;

public:
	Camera lightCamera;

private:
	std::unique_ptr<PostEffect> mShadowMap;
	RenderTexture* mShadowMapRT;

public:
	ShadowMap();
	void Init();
	void Register(const uint32_t size);
	void LightViewUpdate(const Vec3 lightPos);
	void Update();
	void DrawPass();
	void DrawPostEffect();
	void DrawDebugGui();

public:
	void Bind(Object3D& object);
	Camera* GetLightCamera();

private:
	friend Singleton<ShadowMap>;
};

