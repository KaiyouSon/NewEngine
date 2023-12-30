#include "PostEffectManager.h"
#include "EffectManager.h"

PostEffectManager::PostEffectManager() :
	mEffectBloom(std::make_unique<Bloom>()),
	mSkydomeVignette(std::make_unique<Vignette>()),
	mRadialBlur(std::make_unique<RadialBlur>())
{
}

void PostEffectManager::Init()
{
}

void PostEffectManager::Update()
{
	mSkydomeVignette->Update();
	mEffectBloom->Update();
	mRadialBlur->Update();
}

void PostEffectManager::DrawDebugGui()
{
	Gui::BeginWindow("PostEffect");
	mRadialBlur->DrawDebugGui();
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
	//mEffectBloom->DrawPass(Bloom::PassType::GaussianBlur);
	mEffectBloom->DrawPostEffect();
}

// ラジアルブラー
void PostEffectManager::DrawRadialBlur()
{
	mRadialBlur->DrawPostEffect();
}

// ゲッター
Bloom* PostEffectManager::GetEffectBloom()
{
	return mEffectBloom.get();
}

void PostEffectManager::SetRadialBlurCenterPos(const Vec3 worldPos)
{
	Vec2 centerPos = WorldToScreen(worldPos);
	Vec2 uvPos = centerPos / GetWindowSize();
	mRadialBlur->SetCenterPos(uvPos);
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
	mEffectBloom->PrevSceneDraw(Bloom::TexType::HighLumi);
	targetDrawFunc();
	mEffectBloom->PostSceneDraw(Bloom::TexType::HighLumi);

	// 高輝度箇所にブラー
	mEffectBloom->PrevSceneDraw(Bloom::TexType::GaussianBlur);
	mEffectBloom->DrawPass(Bloom::PassType::HighLumi);
	mEffectBloom->PostSceneDraw(Bloom::TexType::GaussianBlur);
	mEffectBloom->PrevSceneDraw(Bloom::TexType::GaussianBlurHalf);
	mEffectBloom->DrawPass(Bloom::PassType::HighLumi);
	mEffectBloom->PostSceneDraw(Bloom::TexType::GaussianBlurHalf);

	// ブラーかけ終わったやつを描画
	mEffectBloom->PrevSceneDraw(Bloom::TexType::Bloom);
	mEffectBloom->DrawPass(Bloom::PassType::GaussianBlur);
	mEffectBloom->PostSceneDraw(Bloom::TexType::Bloom);

	// 現在のシーンの描画
	mEffectBloom->PrevSceneDraw(Bloom::TexType::Target);
	sceneDrawFunc();
	mEffectBloom->PostSceneDraw(Bloom::TexType::Target);
}

void PostEffectManager::RadialBlurDrawPass(
	const std::function<void()>& targetDrawFunc,
	const std::function<void()>& sceneDrawFunc)
{
	mRadialBlur->PrevSceneDraw(RadialBlur::PassType::Target);
	targetDrawFunc();
	mRadialBlur->PostSceneDraw(RadialBlur::PassType::Target);

	mRadialBlur->PrevSceneDraw(RadialBlur::PassType::Finish);
	mRadialBlur->DrawPass(RadialBlur::PassType::Target);
	mRadialBlur->PostSceneDraw(RadialBlur::PassType::Finish);

	mRadialBlur->PrevSceneDraw(RadialBlur::PassType::Scene);
	sceneDrawFunc();
	mRadialBlur->PostSceneDraw(RadialBlur::PassType::Scene);
}

