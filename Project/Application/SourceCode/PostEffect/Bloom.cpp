#include "Bloom.h"

Bloom::Bloom()
{
	// テクスチャ
	mTexs[(uint32_t)PassType::HighLumi] = TextureManager::GetRenderTexture("BloomHighLumi");
	mTexs[(uint32_t)PassType::GaussianBlur] = TextureManager::GetRenderTexture("BloomGaussianBlur");
	mTexs[(uint32_t)PassType::GaussianBlurHalf] = TextureManager::GetRenderTexture("BloomGaussianBlurHalf");
	mTexs[(uint32_t)PassType::Bloom] = TextureManager::GetRenderTexture("Bloom");
	mTexs[(uint32_t)PassType::Bloom1] = TextureManager::GetRenderTexture("Bloom1");
	mTexs[(uint32_t)PassType::Target] = TextureManager::GetRenderTexture("BloomTarget");

	for (uint32_t i = 0; i < mPasses.size(); i++)
	{
		mPasses[i] = std::make_unique<PostEffect>();
		mPasses[i]->pos = GetWindowHalfSize();
	}
	mPasses[(uint32_t)PassType::HighLumi]->AddRenderTexture(mTexs[(uint32_t)PassType::HighLumi]);
	mPasses[(uint32_t)PassType::GaussianBlur]->AddRenderTexture(mTexs[(uint32_t)PassType::GaussianBlur]);
	mPasses[(uint32_t)PassType::GaussianBlurHalf]->AddRenderTexture(mTexs[(uint32_t)PassType::GaussianBlurHalf]);

	// パイプライン
	mPasses[(uint32_t)PassType::HighLumi]->
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("HighLumi"));
	mPasses[(uint32_t)PassType::GaussianBlur]->
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GaussianBlur"));
	mPasses[(uint32_t)PassType::GaussianBlurHalf]->
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GaussianBlur"));

	// サイズ設定
	mPasses[(uint32_t)PassType::GaussianBlurHalf]->SetSize(GetWindowSize());

	// 合成用
	mCompositePass = std::make_unique<PostEffect>();
	mCompositePass->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Composite"));
	mCompositePass->AddRenderTexture(mTexs[(uint32_t)PassType::Bloom]);
	mCompositePass->AddRenderTexture(mTexs[(uint32_t)PassType::Bloom1]);
	mCompositePass->AddRenderTexture(mTexs[(uint32_t)PassType::Target]);
	mCompositePass->pos = GetWindowHalfSize();
	mCompositePass->SetSize(GetWindowSize());

	mHighLumiClamp = Vec2(0.1f, 1.0f);
	mPasses[(uint32_t)PassType::HighLumi]->AddMaterial<Vec2>();
}

void Bloom::Update()
{
	if (Key::GetKeyDown(DIK_S))
	{
		mPasses[(uint32_t)PassType::HighLumi]->SetSize(GetWindowSize());
	}

	for (uint32_t i = 0; i < mPasses.size(); i++)
	{
		mPasses[i]->Update();
	}

	mCompositePass->Update();
}

void Bloom::DrawPostEffect()
{
	mCompositePass->Draw();
}

void Bloom::DrawPass(const PassType passType)
{
	if (passType == PassType::HighLumi)
	{
		mPasses[(uint32_t)PassType::HighLumi]->SetTransferBuffer(2, mHighLumiClamp);
	}

	mPasses[(uint32_t)passType]->Draw();
}

void Bloom::PrevSceneDraw(const PassType passType)
{
	mTexs[(uint32_t)passType]->PrevDrawScene();
}

void Bloom::PostSceneDraw(const PassType passType)
{
	mTexs[(uint32_t)passType]->PostDrawScene();
}

void Bloom::SetHighLumiClmap(const Vec2 highLumiClamp)
{
	mHighLumiClamp = highLumiClamp;
}

Vec2 Bloom::GetHighLumiClmap()
{
	return mHighLumiClamp;
}

