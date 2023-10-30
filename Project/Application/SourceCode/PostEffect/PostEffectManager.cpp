#include "PostEffectManager.h"
#include "EffectManager.h"

PostEffectManager::PostEffectManager() :
	mEffectBloom(std::make_unique<Bloom>()),
	mBossBloom(std::make_unique<Bloom>()),
	mSkydomeVignette(std::make_unique<Vignette>())
{
	//// テクスチャ
	//mTexs[(uint32_t)PassType::HighLumi] = TextureManager::GetRenderTexture("HighLumi");
	//mTexs[(uint32_t)PassType::GaussianBlur] = TextureManager::GetRenderTexture("GaussianBlur");
	//mTexs[(uint32_t)PassType::Bloom] = TextureManager::GetRenderTexture("EffectBloom");
	//mTexs[(uint32_t)PassType::Target] = TextureManager::GetRenderTexture("EffectBloomTarget");

	// エフェクトブルーム
	mEffectBloom->SetRenderTexture(TextureManager::GetRenderTexture("EffectHighLumi"), Bloom::PassType::HighLumi);
	mEffectBloom->SetRenderTexture(TextureManager::GetRenderTexture("EffectGaussianBlur"), Bloom::PassType::GaussianBlur);
	mEffectBloom->SetRenderTexture(TextureManager::GetRenderTexture("EffectBloom"), Bloom::PassType::Bloom);
	mEffectBloom->SetRenderTexture(TextureManager::GetRenderTexture("EffectBloomTarget"), Bloom::PassType::Target);

	// ボスブルーム
	mBossBloom->SetRenderTexture(TextureManager::GetRenderTexture("BossHighLumi"), Bloom::PassType::HighLumi);
	mBossBloom->SetRenderTexture(TextureManager::GetRenderTexture("BossGaussianBlur"), Bloom::PassType::GaussianBlur);
	mBossBloom->SetRenderTexture(TextureManager::GetRenderTexture("BossBloom"), Bloom::PassType::Bloom);
	mBossBloom->SetRenderTexture(TextureManager::GetRenderTexture("BossBloomTarget"), Bloom::PassType::Target);
}

void PostEffectManager::Init()
{
}

void PostEffectManager::Update()
{
	mSkydomeVignette->Update();
	mEffectBloom->Update();
	mBossBloom->Update();
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

// ボスのブルーム	
void PostEffectManager::DrawBossBloom()
{
	mBossBloom->DrawPostEffect();
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
	mEffectBloom->DrawPass(Bloom::PassType::HighLumi);
	mEffectBloom->PostSceneDraw(Bloom::PassType::GaussianBlur);

	// ブラーかけ終わったやつを描画
	mEffectBloom->PrevSceneDraw(Bloom::PassType::Bloom);
	mEffectBloom->DrawPass(Bloom::PassType::GaussianBlur);
	mEffectBloom->PostSceneDraw(Bloom::PassType::Bloom);

	// 現在のシーンの描画
	mEffectBloom->PrevSceneDraw(Bloom::PassType::Target);
	sceneDrawFunc();
	mEffectBloom->PostSceneDraw(Bloom::PassType::Target);
}

// ボスのブルームのパス設定
void PostEffectManager::BossBloomDrawPass(
	const std::function<void()>& targetDrawFunc,
	const std::function<void()>& sceneDrawFunc)
{
	// 高輝度箇所を抽出
	mBossBloom->PrevSceneDraw(Bloom::PassType::HighLumi);
	targetDrawFunc();
	mBossBloom->PostSceneDraw(Bloom::PassType::HighLumi);

	// 高輝度箇所にブラー
	mBossBloom->PrevSceneDraw(Bloom::PassType::GaussianBlur);
	mBossBloom->DrawPass(Bloom::PassType::HighLumi);
	mBossBloom->PostSceneDraw(Bloom::PassType::GaussianBlur);

	// ブラーかけ終わったやつを描画
	mBossBloom->PrevSceneDraw(Bloom::PassType::Bloom);
	mBossBloom->DrawPass(Bloom::PassType::GaussianBlur);
	mBossBloom->PostSceneDraw(Bloom::PassType::Bloom);

	// 現在のシーンの描画
	mBossBloom->PrevSceneDraw(Bloom::PassType::Target);
	sceneDrawFunc();
	mBossBloom->PostSceneDraw(Bloom::PassType::Target);
}

