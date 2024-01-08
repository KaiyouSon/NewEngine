#include "PostEffectManager.h"
#include "EffectManager.h"

PostEffectManager::PostEffectManager() :
	mBloom(std::make_unique<Bloom>()),
	mRadialBlur(std::make_unique<RadialBlur>()),
	mToneMapping(std::make_unique<ToneMapping>()),
	mVignette(std::make_unique<Vignette>()),
	mLensFlare(std::make_unique<LensFlare>())
{
}

void PostEffectManager::Init()
{
	mToneMapping->Init();
}

void PostEffectManager::Update()
{
	mBloom->Update();
	mRadialBlur->Update();
	mToneMapping->Update();
	mVignette->Update();
	mLensFlare->Update();
}

void PostEffectManager::DrawDebugGui()
{
	Gui::BeginWindow("PostEffect");
	mRadialBlur->DrawDebugGui();
	mToneMapping->DrawDebugGui();
	Gui::EndWindow();
}

void PostEffectManager::DrawPostEffect(const PostEffectType type)
{
	switch (type)
	{
	case PostEffectType::Bloom:
		mBloom->DrawPostEffect();
		break;

	case PostEffectType::RadialBlur:
		mRadialBlur->DrawPostEffect();
		break;

	case PostEffectType::ToneMapping:
		mToneMapping->DrawPostEffect();
		break;

	case PostEffectType::Vignette:
		mVignette->DrawPostEffect();
		break;

	case PostEffectType::LensFlare:
		mLensFlare->DrawPostEffect();
		break;
	};
}

// ゲッター
Bloom* PostEffectManager::GetEffectBloom()
{
	return mBloom.get();
}

void PostEffectManager::SetRadialBlurCenterPos(const Vec3 worldPos)
{
	Vec2 centerPos = WorldToScreen(worldPos);
	Vec2 uvPos = centerPos / GetWindowSize();
	mRadialBlur->SetCenterPos(uvPos);
}

///--- パスの設定 ------------------------------------------------------------------------------------------------ ///

// エフェクトのブルームのパス設定
void PostEffectManager::DrawBloomPass(
	const std::function<void()>& targetDrawFunc,
	const std::function<void()>& sceneDrawFunc)
{
	mBloom->DrawPass(targetDrawFunc, sceneDrawFunc);
}

void PostEffectManager::DrawRadialBlurPass(
	const std::function<void()>& targetDrawFunc,
	const std::function<void()>& sceneDrawFunc)
{
	mRadialBlur->DrawPass(targetDrawFunc, sceneDrawFunc);
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

void PostEffectManager::DrawLensFlarePass(
	const std::function<void()>& targetDrawFunc,
	const std::function<void()>& maskDrawFunc)
{
	mLensFlare->DrawPass(targetDrawFunc, maskDrawFunc);
}
