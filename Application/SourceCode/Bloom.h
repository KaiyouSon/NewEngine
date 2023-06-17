#pragma once
#include "NewEngine.h"

class Bloom
{
private:
	std::unique_ptr<PostEffect> postEffect_;
	RenderTexture* tex_;

public:
	Bloom();
	void Update();
	void DrawPostEffect();
	void PrevSceneDraw();
	void PostSceneDraw();
};

