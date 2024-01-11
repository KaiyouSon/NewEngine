#include "RadialBlur.h"
using namespace ConstantBufferData;

RadialBlur::RadialBlur() :
	mCompositePass(std::make_unique<PostEffect>())
{
	mTexes[(uint32_t)PassType::Target] = TextureManager::GetRenderTexture("RadialBlurTarget");
	mTexes[(uint32_t)PassType::Finish] = TextureManager::GetRenderTexture("RadialBlurFinish");
	mTexes[(uint32_t)PassType::Scene] = TextureManager::GetRenderTexture("RadialBlurScene");

	for (uint32_t i = 0; i < (uint32_t)PassType::Size; i++)
	{
		mPasses[i] = std::make_unique<PostEffect>();
		mPasses[i]->pos = GetWindowHalfSize();
	}

	mPasses[(uint32_t)PassType::Target]->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("RadialBlur"));
	mPasses[(uint32_t)PassType::Target]->AddMaterial<CRadialBlur>();

	// テクスチャ設定
	mPasses[(uint32_t)PassType::Target]->AddRenderTexture(mTexes[(uint32_t)PassType::Target]);
	mPasses[(uint32_t)PassType::Finish]->AddRenderTexture(mTexes[(uint32_t)PassType::Finish]);
	mPasses[(uint32_t)PassType::Scene]->AddRenderTexture(mTexes[(uint32_t)PassType::Scene]);

	mCompositePass->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Composite"));
	mCompositePass->AddRenderTexture(mTexes[(uint32_t)PassType::Finish]);
	mCompositePass->AddRenderTexture(mTexes[(uint32_t)PassType::Scene]);
	mCompositePass->pos = GetWindowHalfSize();

	mPasses[(uint32_t)PassType::Target]->SetSize(GetWindowSize());

	mData.strength = 0.0125f;
	mData.loopNum = 30;
}

void RadialBlur::Update()
{
	mPasses[(uint32_t)PassType::Target]->SetTransferBuffer(2, mData);

	for (uint32_t i = 0; i < (uint32_t)PassType::Size; i++)
	{
		mPasses[i]->Update();
	}
	mCompositePass->Update();
}

void RadialBlur::DrawPostEffect()
{
	mCompositePass->Draw();
}

void RadialBlur::DrawPass(
	const std::function<void()>& targetDrawFunc,
	const std::function<void()>& sceneDrawFunc)
{
	mTexes[(uint32_t)PassType::Target]->PrevDrawScene();
	targetDrawFunc();
	mTexes[(uint32_t)PassType::Target]->PostDrawScene();

	mTexes[(uint32_t)PassType::Finish]->PrevDrawScene();
	mPasses[(uint32_t)PassType::Target]->Draw();
	mTexes[(uint32_t)PassType::Finish]->PostDrawScene();

	mTexes[(uint32_t)PassType::Scene]->PrevDrawScene();
	sceneDrawFunc();
	mTexes[(uint32_t)PassType::Scene]->PostDrawScene();
}

void RadialBlur::PrevSceneDraw(const PassType type)
{
	mTexes[(uint32_t)type]->PrevDrawScene();
}

void RadialBlur::PostSceneDraw(const PassType type)
{
	mTexes[(uint32_t)type]->PostDrawScene();
}

void RadialBlur::DrawDebugGui()
{
	if (Gui::DrawCollapsingHeader("RadialBlur"))
	{
		Gui::DrawSlider1("Strength", mData.strength, 0.01f);
		mData.strength = Max<float>(mData.strength, 0.f);

		Gui::DrawSlider1("LoopNum", mData.loopNum, 1.f);
		mData.loopNum = Clamp<float>(mData.loopNum, 1, 1000);
	}
}

void RadialBlur::SetCenterPos(const Vec2 centerPos)
{
	mData.centerPos = centerPos;
}
