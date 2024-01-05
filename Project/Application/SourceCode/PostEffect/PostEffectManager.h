#pragma once
#include "NewEngine.h"
#include "Bloom.h"
#include "Vignette.h"
#include "RadialBlur.h"
#include "ToneMapping.h"

enum class PostEffectType
{
	Bloom,
	RadialBlur,
	ToneMapping,
	Vignette,
};

// 全てのポストエフェクトを管理するクラス
class PostEffectManager
{
private:
	std::unique_ptr<Bloom> mEffectBloom;
	std::unique_ptr<Vignette> mVignette;
	std::unique_ptr<RadialBlur> mRadialBlur;
	std::unique_ptr<ToneMapping> mToneMapping;

public:
	// エフェクトのブルームのパス設定
	void EffectBloomDrawPass(
		const std::function<void()>& targetDrawFunc,
		const std::function<void()>& sceneDrawFunc);

	void RadialBlurDrawPass(
		const std::function<void()>& targetDrawFunc,
		const std::function<void()>& sceneDrawFunc);

	// トーンマッピング
	void DrawToneMappingPass(
		const std::function<void()>& targetDrawFunc);

	// ビネット
	void DrawVignettePass(
		const std::function<void()>& targetDrawFunc,
		const std::function<void()>& maskDrawFunc);

public:
	PostEffectManager();
	void Init();
	void Update();
	void DrawDebugGui();

public:

	// 天球のビネット
	void DrawSkydomeVignette();

	// エフェクトのブルーム	
	void DrawEffectBloom();

	// ラジアルブラー
	void DrawRadialBlur();

	// ポストエフェクトを描画する関数
	void DrawPostEffect(const PostEffectType type);

public:
	Bloom* GetEffectBloom();
	void SetRadialBlurCenterPos(const Vec3 centerPos);

};
