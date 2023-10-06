#pragma once
#include "NewEngine.h"

class Task
{
private:
	std::unique_ptr<PostEffect> postEffect_;
	RenderTexture* tex_;

public:
	Task();
	void Update();
	void DrawPostEffect();
	void PrevSceneDraw();
	void PostSceneDraw();

};
