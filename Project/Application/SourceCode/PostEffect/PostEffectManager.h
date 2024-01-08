#pragma once
#include "NewEngine.h"
#include "Bloom.h"
#include "Vignette.h"
#include "RadialBlur.h"
#include "ToneMapping.h"
#include "LensFlare.h"

enum class PostEffectType
{
	Bloom,
	RadialBlur,
	ToneMapping,
	Vignette,
	LensFlare
};

// 全てのポストエフェクトを管理するクラス
class PostEffectManager
{
private:
	std::unique_ptr<RadialBlur> mRadialBlur;
	std::unique_ptr<Bloom> mBloom;
	std::unique_ptr<ToneMapping> mToneMapping;
	std::unique_ptr<Vignette> mVignette;
	std::unique_ptr<LensFlare> mLensFlare;

public:
	// ラジアルブラー
	void DrawRadialBlurPass(
		const std::function<void()>& targetDrawFunc,
		const std::function<void()>& sceneDrawFunc);

	// ブルーム
	void DrawBloomPass(
		const std::function<void()>& targetDrawFunc,
		const std::function<void()>& sceneDrawFunc);

	// トーンマッピング
	void DrawToneMappingPass(
		const std::function<void()>& targetDrawFunc);

	// ビネット
	void DrawVignettePass(
		const std::function<void()>& targetDrawFunc,
		const std::function<void()>& maskDrawFunc);

	// レンズフレア
	void DrawLensFlarePass(
		const std::function<void()>& targetDrawFunc,
		const std::function<void()>& maskDrawFunc);

public:
	PostEffectManager();
	void Init();
	void Update();
	void DrawDebugGui();

public:
	// ポストエフェクトを描画する関数
	void DrawPostEffect(const PostEffectType type);

public:
	Bloom* GetEffectBloom();
	void SetRadialBlurCenterPos(const Vec3 centerPos);

};
