#include "Vignette.h"
using namespace ConstantBufferData;

Vignette::Vignette() :
	mPostEffect(std::make_unique<PostEffect>())
{
	// テクスチャ
	mTex[(uint32_t)PassType::Target] = TextureManager::GetRenderTexture("VignetteTarget");
	mTex[(uint32_t)PassType::Mask] = TextureManager::GetRenderTexture("VignetteMask");

	// ポストエフェクト
	mPostEffect->pos = GetWindowHalfSize();
	mPostEffect->AddRenderTexture(mTex[(uint32_t)PassType::Target]);
	mPostEffect->AddRenderTexture(mTex[(uint32_t)PassType::Mask]);
	mPostEffect->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Vignette"));
	mPostEffect->AddMaterial<CVignette>();

	// データ
	mVignetteData.range = Vec2(0.2f, 1.7f);
	mVignetteData.color = Color::black;
}

void Vignette::Update()
{
	mPostEffect->SetTransferBuffer(2, mVignetteData);
	mPostEffect->Update();
}

void Vignette::DrawPass(
	const std::function<void()>& targetDrawFunc,
	const std::function<void()>& maskDrawFunc)
{
	// ターゲット
	mTex[(uint32_t)PassType::Target]->PrevDrawScene();
	targetDrawFunc();
	mTex[(uint32_t)PassType::Target]->PostDrawScene();

	// マスク
	mTex[(uint32_t)PassType::Mask]->PrevDrawScene();
	maskDrawFunc();
	mTex[(uint32_t)PassType::Mask]->PostDrawScene();
}

void Vignette::DrawPostEffect()
{
	mPostEffect->Draw();
}

void Vignette::DrawDebugGui()
{
	Gui::DrawColorEdit("Vignette Color", mVignetteData.color);
	mVignetteData.color = mVignetteData.color.To01();
	Gui::DrawSlider2("Vignette Range", mVignetteData.range, 0.01f);
}