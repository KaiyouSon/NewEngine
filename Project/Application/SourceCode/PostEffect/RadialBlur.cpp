#include "stdafx.h"
#include "RadialBlur.h"

RadialBlur::RadialBlur() :
	mPostEffect(std::make_unique<PostEffect>())
{
	mTex = TextureManager::GetRenderTexture("RadialBlur");

	mPostEffect->pos = GetWindowHalfSize();
	mPostEffect->AddRenderTexture(mTex);
	mPostEffect->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("RadialBlur"));
	//mPostEffect->AddMaterial<ConstantBuffer<CRadialBlur>>();
}

void RadialBlur::Update()
{
}

void RadialBlur::DrawPostEffect()
{
}

void RadialBlur::PrevSceneDraw()
{
}

void RadialBlur::PostSceneDraw()
{
}

void RadialBlur::DrawDebugGui()
{
}
