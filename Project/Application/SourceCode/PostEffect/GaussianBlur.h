#pragma once
#include "NewEngine.h"

class GaussianBlur
{
private:
	std::unique_ptr<PostEffect> postEffect_;
	RenderTexture* tex_;

public:
	GaussianBlur();
	static void CreateGraphicsPipeline();
	void Update();
	void DrawPostEffect();
	void PrevSceneDraw();
	void PostSceneDraw();
};
