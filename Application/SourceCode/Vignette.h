#pragma once
#include "NewEngine.h"

class Vignette
{
private:
	std::unique_ptr<PostEffect> postEffect;
	RenderTexture* vignetteTex;

public:
	Vignette();
	void Update();
	void DrawPostEffect();
	void PrevSceneDraw();
	void PostSceneDraw();

};

