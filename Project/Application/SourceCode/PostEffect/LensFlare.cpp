#include "LensFlare.h"
using namespace ConstantBufferData;

LensFlare::LensFlare() :
	mPass(std::make_unique<PostEffect>())
{
	mTexes[(uint32_t)PassType::Target] = TextureManager::GetRenderTexture("LensFlareTarget");
	mTexes[(uint32_t)PassType::Mask] = TextureManager::GetRenderTexture("LensFlareMask");

	mPass->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("LensFlare"));

	// テクスチャ設定
	mPass->AddRenderTexture(mTexes[(uint32_t)PassType::Target]);
	mPass->AddRenderTexture(mTexes[(uint32_t)PassType::Mask]);

	// 座標
	mPass->pos = GetWindowHalfSize();
}

void LensFlare::Update()
{
	mPass->Update();
}

void LensFlare::DrawPostEffect()
{
	mPass->Draw();
}

void LensFlare::DrawPass(
	const std::function<void()>& targetDrawFunc,
	const std::function<void()>& maskDrawFunc)
{
	mTexes[(uint32_t)PassType::Target]->PrevDrawScene();
	targetDrawFunc();
	mTexes[(uint32_t)PassType::Target]->PostDrawScene();

	mTexes[(uint32_t)PassType::Mask]->PrevDrawScene();
	maskDrawFunc();
	mTexes[(uint32_t)PassType::Mask]->PostDrawScene();
}

void LensFlare::PrevSceneDraw(const PassType type)
{
	mTexes[(uint32_t)type]->PrevDrawScene();
}

void LensFlare::PostSceneDraw(const PassType type)
{
	mTexes[(uint32_t)type]->PostDrawScene();
}

void LensFlare::DrawDebugGui()
{
	//if (Gui::DrawCollapsingHeader("LensFlare"))
	//{
	//	Gui::DrawSlider1("Strength", mData.strength, 0.01f);
	//	mData.strength = Max<float>(mData.strength, 0.f);

	//	Gui::DrawSlider1("LoopNum", mData.loopNum, 1.f);
	//	mData.loopNum = Clamp<float>(mData.loopNum, 1, 1000);
	//}
}

void LensFlare::SetCenterPos(const Vec2 centerPos)
{
	centerPos;
	//mData.centerPos = centerPos;
}
