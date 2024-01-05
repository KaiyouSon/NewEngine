#pragma once
#include "NewEngine.h"

// ラジアルブラーのクラス
class RadialBlur
{
public:
	enum class PassType
	{
		Target,
		Finish,
		Scene,

		Size
	};

private:
	std::array<std::unique_ptr<PostEffect>, (uint32_t)PassType::Size> mPasses;
	std::array<RenderTexture*, (uint32_t)PassType::Size> mTexes;
	ConstantBufferData::CRadialBlur mData;

	std::unique_ptr<PostEffect> mCompositePass;

public:
	RadialBlur();
	void Update();
	void DrawPostEffect();
	void DrawPass(const std::function<void()>& targetDrawFunc, const std::function<void()>& sceneDrawFunc);
	void PrevSceneDraw(const PassType type);
	void PostSceneDraw(const PassType type);
	void DrawDebugGui();

	void SetCenterPos(const Vec2 centerPos);
};

