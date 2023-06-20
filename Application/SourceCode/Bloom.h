#pragma once
#include "NewEngine.h"

class Bloom
{
private:
	std::unique_ptr<PostEffect> highLumi_;
	std::unique_ptr<PostEffect> blur_;
	std::unique_ptr<PostEffect> bloom_;
	std::array<RenderTexture*, 4> texs_;

public:
	Bloom();
	static void CreateGraphicsPipeline();
	void Update();
	void DrawPostEffect(const uint32_t index);
	void PrevSceneDraw(const uint32_t index);
	void PostSceneDraw(const uint32_t index);
};

