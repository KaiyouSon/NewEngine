#pragma once
#include "NewEngine.h"

class Bloom
{
public:
	enum class TexType
	{
		HighLumi,
		Blur,
		Bloom
	};

private:
	std::unique_ptr<PostEffect> highLumi_;
	std::unique_ptr<PostEffect> blur_;
	std::unique_ptr<PostEffect> bloom_;
	std::array<RenderTexture*, 3> texs_;

public:
	Bloom();
	static void CreateGraphicsPipeline();
	void Update();
	void DrawPostEffect(const TexType texType);
	void PrevSceneDraw(const TexType texType);
	void PostSceneDraw(const TexType texType);
	void DrawDebugGui();
};

