#pragma once
#include "NewEngine.h"
#include "ShadowObj.h"

template<typename T> class Singleton;

// シャドウマップ制御するクラス
class ShadowMap : public Singleton<ShadowMap>
{
private:
	std::vector<ShadowObj> mShadowObjs;
	std::vector<Transform> mParents;
	uint32_t mIndex;

public:
	Camera mLightCamera;

private:
	std::unique_ptr<PostEffect> mShadowMap;
	RenderTexture* mShadowMapRT;

public:
	ShadowMap();
	void Init();
	void Register(const uint32_t size);
	void Update();
	void RenderTextureSetting();
	void DrawModel();
	void DrawPostEffect();

public:
	void Bind(Object3D& object);
	Camera GetLightCamera();

private:
	friend Singleton<ShadowMap>;
};

