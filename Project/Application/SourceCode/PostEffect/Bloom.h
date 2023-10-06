#pragma once
#include "NewEngine.h"

class Bloom
{
public:
	enum class PassType
	{
		// •`‰æ—p
		HighLumi,
		GaussianBlur,
		Bloom,
		Target,

		Size
	};

private:
	std::array<std::unique_ptr<PostEffect>, (uint32_t)PassType::Size> mPasses;
	std::array<RenderTexture*, (uint32_t)PassType::Size> mTexs;
	std::unique_ptr<PostEffect> mCompositePass;

public:
	Bloom();
	static void CreateGraphicsPipeline();
	void Update();
	void DrawPostEffect();
	void DrawPass(const PassType passType);
	void PrevSceneDraw(const PassType passType);
	void PostSceneDraw(const PassType passType);
	void DrawDebugGui();
};

