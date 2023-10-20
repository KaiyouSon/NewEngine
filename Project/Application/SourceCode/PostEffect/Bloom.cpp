#include "Bloom.h"

Bloom::Bloom()
{
	// 繝・け繧ｹ繝√Ε繝ｼ縺ｮ繧ｻ繝・ヨ
	mTexs[(uint32_t)PassType::HighLumi] = TextureManager::GetRenderTexture("HighLumi");
	mTexs[(uint32_t)PassType::GaussianBlur] = TextureManager::GetRenderTexture("GaussianBlur");
	mTexs[(uint32_t)PassType::Bloom] = TextureManager::GetRenderTexture("Bloom");
	mTexs[(uint32_t)PassType::Target] = TextureManager::GetRenderTexture("EffectBloom");

	// 
	for (uint32_t i = 0; i < mPasses.size(); i++)
	{
		mPasses[i] = std::make_unique<PostEffect>();
		mPasses[i]->pos = GetWindowHalfSize();
	}
	mPasses[(uint32_t)PassType::HighLumi]->AddRenderTexture(mTexs[(uint32_t)PassType::HighLumi]);
	mPasses[(uint32_t)PassType::GaussianBlur]->AddRenderTexture(mTexs[(uint32_t)PassType::GaussianBlur]);

	// 繝代う繝励Λ繧､繝ｳ險ｭ螳・
	mPasses[(uint32_t)PassType::HighLumi]->
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("HighLumi"));
	mPasses[(uint32_t)PassType::GaussianBlur]->
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GaussianBlur"));

	// 蜷域・逕ｨ繝代せ
	mCompositePass = std::make_unique<PostEffect>();
	mCompositePass->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Composite"));
	mCompositePass->AddRenderTexture(mTexs[(uint32_t)PassType::Bloom]);
	mCompositePass->AddRenderTexture(mTexs[(uint32_t)PassType::Target]);
	mCompositePass->pos = GetWindowHalfSize();

	mHighLumiClamp = Vec2(0.2f, 0.6f);
	mPasses[(uint32_t)PassType::HighLumi]->AddMaterial<Vec2>();
}

void Bloom::Update()
{
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

