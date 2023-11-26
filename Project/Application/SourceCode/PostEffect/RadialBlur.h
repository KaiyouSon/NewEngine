#pragma once
#include "NewEngine.h"

// ラジアルブラーのクラス
class RadialBlur
{
private:
	std::unique_ptr<PostEffect> mPostEffect;
	RenderTexture* mTex;

public:
	RadialBlur();
	void Update();
	void DrawPostEffect();
	void PrevSceneDraw();
	void PostSceneDraw();
	void DrawDebugGui();
};

