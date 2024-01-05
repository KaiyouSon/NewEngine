#pragma once
#include "NewEngine.h"

// ビネットのクラス
class Vignette
{
public:
	enum class PassType
	{
		Target,
		Mask,

		Size
	};


private:
	std::array<RenderTexture*, (uint32_t)PassType::Size> mTex;
	std::unique_ptr<PostEffect> mPostEffect;
	ConstantBufferData::CVignette mVignetteData;

public:
	Vignette();
	void Update();
	void DrawPass(const std::function<void()>& targetDrawFunc, const std::function<void()>& maskDrawFunc);
	void DrawPostEffect();
	void DrawDebugGui();
};
