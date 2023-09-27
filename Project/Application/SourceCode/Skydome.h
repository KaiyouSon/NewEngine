#pragma once
#include "NewEngine.h"
#include "Cloud.h"

class Skydome
{
private:
	std::unique_ptr<Object3D> mSkydome;
	std::vector<std::unique_ptr<Cloud>> mClouds;
	std::unique_ptr<PostEffect> mPostEffect;
	RenderTexture* mRenderTexture;
	bool isVignette;
	ConstantBufferData::CVignette mVignetteData;

	bool test;

public:
	static void CreateGraphicsPipeline();

	Skydome();
	void Init();
	void Update();
	void RenderTextureSetting();
	void Draw();
	void DrawDebugGui();
};

