#include "RadialBlur.h"
using namespace ConstantBufferData;

RadialBlur::RadialBlur() :
	mCompositePass(std::make_unique<PostEffect>())
{
	mTex[(uint32_t)PassType::Target] = TextureManager::GetRenderTexture("RadialBlurTarget");
	mTex[(uint32_t)PassType::Mask] = TextureManager::GetRenderTexture("RadialBlurMask");
	mTex[(uint32_t)PassType::Finish] = TextureManager::GetRenderTexture("RadialBlurFinish");
	mTex[(uint32_t)PassType::Scene] = TextureManager::GetRenderTexture("RadialBlurScene");

	for (uint32_t i = 0; i < (uint32_t)PassType::Size; i++)
	{
		mPostEffect[i] = std::make_unique<PostEffect>();
		mPostEffect[i]->pos = GetWindowHalfSize();
	}

	mPostEffect[(uint32_t)PassType::Target]->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("RadialBlur"));
	mPostEffect[(uint32_t)PassType::Target]->AddMaterial<CRadialBlur>();

	// テクスチャ設定
	mPostEffect[(uint32_t)PassType::Target]->AddRenderTexture(mTex[(uint32_t)PassType::Target]);
	mPostEffect[(uint32_t)PassType::Target]->AddRenderTexture(mTex[(uint32_t)PassType::Mask]);
	mPostEffect[(uint32_t)PassType::Finish]->AddRenderTexture(mTex[(uint32_t)PassType::Finish]);
	mPostEffect[(uint32_t)PassType::Scene]->AddRenderTexture(mTex[(uint32_t)PassType::Scene]);

	mCompositePass->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Composite"));
	mCompositePass->AddRenderTexture(mTex[(uint32_t)PassType::Finish]);
	mCompositePass->AddRenderTexture(mTex[(uint32_t)PassType::Scene]);
	mCompositePass->pos = GetWindowHalfSize();

	mData.strength = 0.01f;
	mData.loopNum = 100;
}

void RadialBlur::Update()
{
	mPostEffect[(uint32_t)PassType::Target]->SetTransferBuffer(2, mData);

	for (uint32_t i = 0; i < (uint32_t)PassType::Size; i++)
	{
		mPostEffect[i]->Update();
	}
	mCompositePass->Update();
}

void RadialBlur::DrawPostEffect()
{
	mCompositePass->Draw();
}

void RadialBlur::DrawPass(const PassType type)
{
	mPostEffect[(uint32_t)type]->Draw();
}

void RadialBlur::PrevSceneDraw(const PassType type)
{
	mTex[(uint32_t)type]->PrevDrawScene();
}

void RadialBlur::PostSceneDraw(const PassType type)
{
	mTex[(uint32_t)type]->PostDrawScene();
}

void RadialBlur::DrawDebugGui()
{
	Gui::BeginWindow("Radial Blur");

	Gui::DrawSlider1("Strength", mData.strength, 0.01f);
	mData.strength = Max<float>(mData.strength, 0.f);

	Gui::DrawSlider1("LoopNum", mData.loopNum, 1.f);
	mData.loopNum = Clamp<float>(mData.loopNum, 1, 1000);

	Gui::EndWindow();
}

void RadialBlur::SetCenterPos(const Vec2 centerPos)
{
	mData.centerPos = centerPos;
}
