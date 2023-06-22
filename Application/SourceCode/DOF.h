#pragma once
#include "NewEngine.h"

class DOF
{
private:
	std::unique_ptr<PostEffect> dof_;
	RenderTexture* tex_;

public:
	float focusWidth;
	float focusDepth;

public:
	DOF();
	static void CreateGraphicsPipeline();
	void Update();
	void DrawPostEffect();
	void PrevSceneDraw();
	void PostSceneDraw();
};

