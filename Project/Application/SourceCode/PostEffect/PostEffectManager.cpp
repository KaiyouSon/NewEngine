#include "PostEffectManager.h"
#include "EffectManager.h"

PostEffectManager::PostEffectManager() :
	mEffectBloom(std::make_unique<Bloom>()),
	mRadialBlur(std::make_unique<RadialBlur>()),
	mToneMapping(std::make_unique<ToneMapping>()),
	mVignette(std::make_unique<Vignette>())
{
}

void PostEffectManager::Init()
{
	mToneMapping->Init();
}

void PostEffectManager::Update()
{
	mEffectBloom->Update();
	mRadialBlur->Update();
	mToneMapping->Update();
	mVignette->Update();
}

void PostEffectManager::DrawDebugGui()
{
	Gui::BeginWindow("PostEffect");
	mRadialBlur->DrawDebugGui();
	mToneMapping->DrawDebugGui();
	Gui::EndWindow();
}

// 天球のビネット
void PostEffectManager::DrawSkydomeVignette()
{
	mVignette->DrawPostEffect();
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

void PostEffectManager::DrawPostEffect(const PostEffectType type)
{
	switch (type)
	{
	case PostEffectType::ToneMapping:
		mToneMapping->DrawPostEffect();
		break;

	case PostEffectType::Vignette:
		mVignette->DrawPostEffect();
		break;
	};

	type;
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

void PostEffectManager::DrawToneMappingPass(
	const std::function<void()>& targetDrawFunc)
{
	mToneMapping->DrawPass(targetDrawFunc);
}

void PostEffectManager::DrawVignettePass(
	const std::function<void()>& targetDrawFunc,
	const std::function<void()>& maskDrawFunc)
{
	mVignette->DrawPass(targetDrawFunc, maskDrawFunc);
}
