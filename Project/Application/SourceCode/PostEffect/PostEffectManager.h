#pragma once
#include "NewEngine.h"
#include "Bloom.h"
#include "Vignette.h"

// 全てのポストエフェクトを管理するクラス
class PostEffectManager
{
private:
	std::unique_ptr<Bloom> mEffectBloom;
	std::unique_ptr<Vignette> mSkydomeVignette;

public:
	// 天球のビネットのパス設定
	void SkydomeVignetteDrawPass(
		const std::function<void()>& targetDrawFunc);

	// エフェクトのブルームのパス設定
	void EffectBloomDrawPass(
		const std::function<void()>& targetDrawFunc,
		const std::function<void()>& sceneDrawFunc);

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

public:
	Bloom* GetEffectBloom();

};
