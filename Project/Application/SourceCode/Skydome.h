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

public:
	Skydome();
	void Init();
	void Update();
	void Draw();
	void DrawDebugGui();
};

