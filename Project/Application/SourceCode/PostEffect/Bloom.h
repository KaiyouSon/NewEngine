#pragma once
#include "NewEngine.h"

// ブルームのクラス
class Bloom
{
public:
	enum class PassType
	{
		// パスの種類
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

	Vec2 mHighLumiClamp;

public:
	bool isBloom0;
	bool isBloom1;

public:
	Bloom();
	void Update();
	void DrawPostEffect();
	void DrawPass(const PassType passType);
	void PrevSceneDraw(const PassType passType);
	void PostSceneDraw(const PassType passType);

public:
	void SetHighLumiClmap(const Vec2 highLumiClamp);

public:
	Vec2 GetHighLumiClmap();
};