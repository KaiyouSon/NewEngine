#include "Vignette.h"
using namespace ConstantBufferData;

Vignette::Vignette() :
	mPostEffect(std::make_unique<PostEffect>())
{
	mRenderTexture = TextureManager::GetRenderTexture("Vignette");

	mPostEffect->pos = GetWindowHalfSize();
	mPostEffect->AddRenderTexture(mRenderTexture);
	mPostEffect->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Vignette"));
	mPostEffect->AddMaterial<ConstantBuffer<CVignette>>();

	mVignetteData.range = Vec2(0.2f, 1.7f);
	mVignetteData.color = Color::black;
}

void Vignette::Update()
{
	mPostEffect->SetTransferBuffer(2, mVignetteData);
	mPostEffect->Update();
}

void Vignette::DrawPostEffect()
{
	mPostEffect->Draw();
}

void Vignette::PrevSceneDraw()
{
	mRenderTexture->PrevDrawScene();
}

void Vignette::PostSceneDraw()
{
	mRenderTexture->PostDrawScene();
}

void Vignette::DrawDebugGui()
{
	Gui::DrawColorEdit("Vignette Color", mVignetteData.color);
	mVignetteData.color = mVignetteData.color.To01();
	Gui::DrawSlider2("Vignette Range", mVignetteData.range, 0.01f);
}