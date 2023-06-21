#pragma once
#include "NewEngine.h"

class GaussainBlur
{
private:
	std::unique_ptr<PostEffect> postEffect_;
	RenderTexture* tex_;

public:
	GaussainBlur();
	static void CreateGraphicsPipeline();
	void Update();
	void DrawPostEffect();
	void PrevSceneDraw();
	void PostSceneDraw();
};

