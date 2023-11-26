#include "PostEffectManager.h"
#include "EffectManager.h"

PostEffectManager::PostEffectManager() :
	mEffectBloom(std::make_unique<Bloom>()),
	mSkydomeVignette(std::make_unique<Vignette>())
{
}

void PostEffectManager::Init()
{
}

void PostEffectManager::Update()
{
	mSkydomeVignette->Update();
	mEffectBloom->Update();
}

void PostEffectManager::DrawDebugGui()
{
	Gui::BeginWindow("PostEffect");
	if (Gui::DrawCollapsingHeader("Bloom"))
	{
		Gui::DrawCheckBox("Bloom0", &mEffectBloom->isBloom0);
		Gui::DrawCheckBox("Bloom1", &mEffectBloom->isBloom1);
	}
	Gui::EndWindow();
}

// 天球のビネット
void PostEffectManager::DrawSkydomeVignette()
{
	mSkydomeVignette->DrawPostEffect();
}

// エフェクトのブルーム
void PostEffectManager::DrawEffectBloom()
{
	mEffectBloom->DrawPostEffect();
}

// ゲッター
Bloom* PostEffectManager::GetEffectBloom()
{
	return mEffectBloom.get();
}

///--- パスの設定 ------------------------------------------------------------------------------------------------ ///

// 天球のビネットのパス設定
void PostEffectManager::SkydomeVignetteDrawPass(
	const std::function<void()>& targetDrawFunc)
{
	mSkydomeVignette->PrevSceneDraw();
	targetDrawFunc();
	mSkydomeVignette->PostSceneDraw();
}

// エフェクトのブルームのパス設定
void PostEffectManager::EffectBloomDrawPass(
	const std::function<void()>& targetDrawFunc,
	const std::function<void()>& sceneDrawFunc)
{
	// 高輝度箇所を抽出
	mEffectBloom->PrevSceneDraw(Bloom::PassType::HighLumi);
	targetDrawFunc();
	mEffectBloom->PostSceneDraw(Bloom::PassType::HighLumi);

	// 高輝度箇所にブラー
	mEffectBloom->PrevSceneDraw(Bloom::PassType::GaussianBlur);
	if (mEffectBloom->isBloom0 == true)
	{
	}
	mEffectBloom->DrawPass(Bloom::PassType::HighLumi);
	mEffectBloom->PostSceneDraw(Bloom::PassType::GaussianBlur);

	// ブラーかけ終わったやつを描画
	mEffectBloom->PrevSceneDraw(Bloom::PassType::Bloom);
	if (mEffectBloom->isBloom0 == true)
	{
	}
	mEffectBloom->DrawPass(Bloom::PassType::GaussianBlur);
	mEffectBloom->PostSceneDraw(Bloom::PassType::Bloom);

	// 現在のシーンの描画
	mEffectBloom->PrevSceneDraw(Bloom::PassType::Target);
	sceneDrawFunc();
	mEffectBloom->PostSceneDraw(Bloom::PassType::Target);
}

