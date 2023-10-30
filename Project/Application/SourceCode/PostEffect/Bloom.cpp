#include "Bloom.h"

Bloom::Bloom()
{
	for (uint32_t i = 0; i < mPasses.size(); i++)
	{
		mPasses[i] = std::make_unique<PostEffect>();
		mPasses[i]->pos = GetWindowHalfSize();
	}

	// パイプライン
	mPasses[(uint32_t)PassType::HighLumi]->
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("HighLumi"));
	mPasses[(uint32_t)PassType::GaussianBlur]->
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GaussianBlur"));

	// 合成用
	mCompositePass = std::make_unique<PostEffect>();
	mCompositePass->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Composite"));
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

void Bloom::SetRenderTexture(RenderTexture* renderTexture, const PassType passType)
{
	mTexs[(uint32_t)passType] = renderTexture;
	switch (passType)
	{
	case PassType::HighLumi:
		mPasses[(uint32_t)PassType::HighLumi]->AddRenderTexture(mTexs[(uint32_t)PassType::HighLumi]);
		break;

	case PassType::GaussianBlur:
		mPasses[(uint32_t)PassType::GaussianBlur]->AddRenderTexture(mTexs[(uint32_t)PassType::GaussianBlur]);
		break;

	case PassType::Bloom:
		mCompositePass->AddRenderTexture(mTexs[(uint32_t)PassType::Bloom]);
		break;

	case PassType::Target:
		mCompositePass->AddRenderTexture(mTexs[(uint32_t)PassType::Target]);
		break;
	}

}

Vec2 Bloom::GetHighLumiClmap()
{
	return mHighLumiClamp;
}

