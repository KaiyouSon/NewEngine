#include "ToneMapping.h"
#include "PostEffect.h"
using namespace ConstantBufferData;

ToneMapping::ToneMapping() :
	mPostEffect(std::make_unique<PostEffect>())
{
	mTexture = TextureManager::GetRenderTexture("ToneMapping");
	mPostEffect->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ToneMapping"));
	mPostEffect->AddRenderTexture(mTexture);
	mPostEffect->AddMaterial<CToneMapping>();
}

void ToneMapping::Load()
{
	TextureManager::CreateRenderTexture(GetWindowSize(), "ToneMapping");
}

void ToneMapping::Init()
{
	mPostEffect->pos = GetWindowHalfSize();

	mData.gain = 1.25f;
	mData.gain.z = 1.75f;
	mData.offset = -0.025f;
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

void ToneMapping::DrawPostEffect()
{
	mPostEffect->Draw();
}

void ToneMapping::DrawDebugGui()
{
	if (Gui::DrawCollapsingHeader("ToneMapping"))
	{
		Gui::DrawSlider3("Gain", mData.gain, 0.01f);
		Gui::DrawSlider3("Offset", mData.offset, 0.01f);
	}
}
