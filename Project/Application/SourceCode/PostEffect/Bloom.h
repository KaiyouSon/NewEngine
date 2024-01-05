#pragma once
#include "NewEngine.h"

// ブルームのクラス
class Bloom
{
public:
	enum class TexType
	{
		// パスの種類
		HighLumi,
		GaussianBlur,
		GaussianBlurHalf,
		Bloom,
		Target,

		Size
	};

	enum class PassType
	{
		// パスの種類
		HighLumi,
		GaussianBlur,
		Bloom,

		Size
	};

private:
	std::array<std::unique_ptr<PostEffect>, (uint32_t)PassType::Size> mPasses;
	std::array<RenderTexture*, (uint32_t)TexType::Size> mTexs;
	Vec2 mHighLumiClamp;

public:
	Bloom();
	void Update();
	void DrawPostEffect();
	void DrawPass(const std::function<void()>& targetDrawFunc, const std::function<void()>& sceneDrawFunc);
	void PrevSceneDraw(const TexType passType);
	void PostSceneDraw(const TexType passType);

public:
	void SetHighLumiClmap(const Vec2 highLumiClamp);

public:
	Vec2 GetHighLumiClmap();
};