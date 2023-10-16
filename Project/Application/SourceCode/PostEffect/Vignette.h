#pragma once
#include "NewEngine.h"

class Vignette
{
private:
	std::unique_ptr<PostEffect> mPostEffect;
	RenderTexture* mRenderTexture;
	ConstantBufferData::CVignette mVignetteData;

public:
	Vignette();
	void Update();
	void DrawPostEffect();
	void PrevSceneDraw();
	void PostSceneDraw();
	void DrawDebugGui();
};
