#pragma once
#include "NewEngine.h"

class ShadowMap
{
private:
	static std::vector<Object3D> sObjShadows;
	static uint32_t sIndex;
	static Camera sLightCamera;

private:
	std::unique_ptr<PostEffect> mCurrentScene;
	RenderTexture* mRenderTex;

public:
	ShadowMap();
	void Init();
	void Update();
	void RenderTextureSetting();
	void DrawModel();
	void DrawPostEffect();

public:
	static void Register();
	static void Bind(Object3D& object);
	static Camera GetLightCamera();
};

