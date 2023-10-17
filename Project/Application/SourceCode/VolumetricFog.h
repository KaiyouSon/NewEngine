#pragma once
#include "NewEngine.h"

class VolumetricFog
{
private:
	std::unique_ptr<PostEffect> mPostEffect;
	RenderTexture* mRenderTexture;

public:
	VolumetricFog();
	void Update();
	void Draw();
	void PrevDrawScene();
	void PostDrawScene();
};