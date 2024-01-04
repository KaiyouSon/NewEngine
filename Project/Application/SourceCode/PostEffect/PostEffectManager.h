#pragma once
#include "NewEngine.h"
#include "Bloom.h"
#include "Vignette.h"
#include "RadialBlur.h"
#include "ToonMapping.h"

enum class PostEffectType
{
	Bloom,
	Vignette,
	RadialBlur,
	ToonMapping
};

// 全てのポストエフェクトを管理するクラス
class PostEffectManager
{
private:
	std::unique_ptr<Bloom> mEffectBloom;
	std::unique_ptr<Vignette> mSkydomeVignette;
	std::unique_ptr<RadialBlur> mRadialBlur;
	std::unique_ptr<ToonMapping> mToonMapping;

public:
	// 天球のビネットのパス設定
	void SkydomeVignetteDrawPass(
		const std::function<void()>& targetDrawFunc);

	// エフェクトのブルームのパス設定
	void EffectBloomDrawPass(
		const std::function<void()>& targetDrawFunc,
		const std::function<void()>& sceneDrawFunc);

	void RadialBlurDrawPass(
		const std::function<void()>& targetDrawFunc,
		const std::function<void()>& sceneDrawFunc);

	// トーンマッピング
	void DrawToonMappingPass(
		const std::function<void()>& targetDrawFunc);

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
