#include "Bloom.h"

Bloom::Bloom()
{
	// テクスチャ
	mTexs[(uint32_t)TexType::HighLumi] = TextureManager::GetRenderTexture("BloomHighLumi");
	mTexs[(uint32_t)TexType::GaussianBlur] = TextureManager::GetRenderTexture("BloomGaussianBlur");
	mTexs[(uint32_t)TexType::GaussianBlurHalf] = TextureManager::GetRenderTexture("BloomGaussianBlurHalf");
	mTexs[(uint32_t)TexType::Bloom] = TextureManager::GetRenderTexture("Bloom");
	mTexs[(uint32_t)TexType::Target] = TextureManager::GetRenderTexture("BloomTarget");

	for (uint32_t i = 0; i < mPasses.size(); i++)
	{
		mPasses[i] = std::make_unique<PostEffect>();
		mPasses[i]->pos = GetWindowHalfSize();
	}
	mPasses[(uint32_t)PassType::HighLumi]->AddRenderTexture(mTexs[(uint32_t)TexType::HighLumi]);
	mPasses[(uint32_t)PassType::GaussianBlur]->AddRenderTexture(mTexs[(uint32_t)TexType::GaussianBlur]);
	mPasses[(uint32_t)PassType::GaussianBlur]->AddRenderTexture(mTexs[(uint32_t)TexType::GaussianBlurHalf]);

	// パイプライン
	mPasses[(uint32_t)PassType::HighLumi]->
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("HighLumi"));
	mPasses[(uint32_t)PassType::GaussianBlur]->
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GaussianBlur"));

	// 合成用
	mPasses[(uint32_t)PassType::Bloom] = std::make_unique<PostEffect>();
	mPasses[(uint32_t)PassType::Bloom]->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Composite"));
	mPasses[(uint32_t)PassType::Bloom]->AddRenderTexture(mTexs[(uint32_t)TexType::Bloom]);
	mPasses[(uint32_t)PassType::Bloom]->AddRenderTexture(mTexs[(uint32_t)TexType::Target]);
	mPasses[(uint32_t)PassType::Bloom]->pos = GetWindowHalfSize();
	mPasses[(uint32_t)PassType::Bloom]->SetSize(GetWindowSize());

	mHighLumiClamp = Vec2(0.1f, 1.0f);
	mPasses[(uint32_t)PassType::HighLumi]->AddMaterial<Vec2>();

	isBloom0 = true;
	isBloom1 = true;
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
}

void Bloom::DrawPostEffect()
{
	mPasses[(uint32_t)PassType::Bloom]->Draw();
}

void Bloom::DrawPass(const PassType passType)
{
	if (passType == PassType::HighLumi)
	{
		mPasses[(uint32_t)PassType::HighLumi]->SetTransferBuffer(2, mHighLumiClamp);
	}

	mPasses[(uint32_t)passType]->Draw();
}

void Bloom::PrevSceneDraw(const TexType passType)
{
	mTexs[(uint32_t)passType]->PrevDrawScene();
}

void Bloom::PostSceneDraw(const TexType passType)
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

