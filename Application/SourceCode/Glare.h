#pragma once
#include "NewEngine.h"

class Glare
{
private:
	std::unique_ptr<PostEffect> highLumi_;
	std::unique_ptr<PostEffect> dotFilter_;
	std::unique_ptr<PostEffect> blur45_;
	std::unique_ptr<PostEffect> blur135_;
	std::unique_ptr<PostEffect> glare_;
	std::unique_ptr<PostEffect> result_;
	std::array<RenderTexture*, 6> texs_;

	ConstantBufferData::CLineBlur lineBlurData1_;
	ConstantBufferData::CLineBlur lineBlurData2_;

public:
	Glare();
	static void CreateGraphicsPipeline();
	void Update();
	void DrawPostEffect(const uint32_t index);
	void PrevSceneDraw(const uint32_t index);
	void PostSceneDraw(const uint32_t index);
	void DrawDebugGui();
};

