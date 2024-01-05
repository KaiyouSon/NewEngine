#include "ToneMapping.h"
#include "PostEffect.h"
using namespace ConstantBufferData;

ToneMapping::ToneMapping() :
	mPostEffect(std::make_unique<PostEffect>())
{
}

void ToneMapping::Load()
{
	TextureManager::CreateRenderTexture(GetWindowSize(), "ToneMapping");
}

void ToneMapping::Init()
{
	mTexture = TextureManager::GetRenderTexture("ToneMapping");
	mPostEffect->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ToneMapping"));
	mPostEffect->AddRenderTexture(mTexture);
	mPostEffect->AddMaterial<CToneMapping>();
	mPostEffect->pos = GetWindowHalfSize();

	mData.gain = 1.25f;
	mData.offset = -0.015f;
}

void ToneMapping::Update()
{
	mPostEffect->SetTransferBuffer(2, mData);
	mPostEffect->Update();
}

void ToneMapping::DrawPass(const std::function<void()>& targetDrawFunc)
{
	mTexture->PrevDrawScene();
	targetDrawFunc();
	mTexture->PostDrawScene();
}

void ToneMapping::Draw()
{
	mPostEffect->Draw();
}

void ToneMapping::DrawDebugGui()
{
	if (Gui::DrawCollapsingHeader("ToneMapping"))
	{
		Gui::DrawSlider1("Gain", mData.gain, 0.01f);
		Gui::DrawSlider1("Offset", mData.offset, 0.01f);
	}
}
