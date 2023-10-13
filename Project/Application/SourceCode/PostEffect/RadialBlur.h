#pragma once
#include "NewEngine.h"

class RadialBlur
{
private:
	std::unique_ptr<PostEffect> postEffect_;
	RenderTexture* tex_;
	ConstantBufferData::CRadialBlur data_;

public:
	RadialBlur();
	static void CreateGraphicsPipeline();
	void Update();
	void DrawPostEffect();
	void PrevSceneDraw();
	void PostSceneDraw();
	void DrawDebugGui();
};
