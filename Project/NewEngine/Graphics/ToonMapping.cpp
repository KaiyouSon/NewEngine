#include "ToonMapping.h"
#include "PostEffect.h"
using namespace ConstantBufferData;

ToonMapping::ToonMapping() :
	mPostEffect(std::make_unique<PostEffect>())
{
}

void ToonMapping::Load()
{
	TextureManager::CreateRenderTexture(GetWindowSize(), "ToonMapping");
}

void ToonMapping::Init()
{
	mTexture = TextureManager::GetRenderTexture("ToonMapping");
	mPostEffect->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ToonMapping"));
	mPostEffect->AddRenderTexture(mTexture);
	mPostEffect->AddMaterial<CToonMapping>();
	mPostEffect->pos = GetWindowHalfSize();

	mData.gain = 0.01f;
	mData.offset = 0.01f;
}

void ToonMapping::Update()
{
	mPostEffect->SetTransferBuffer(2, mData);
	mPostEffect->Update();
}

void ToonMapping::DrawPass(const std::function<void()>& targetDrawFunc)
{
	mTexture->PrevDrawScene();
	targetDrawFunc();
	mTexture->PostDrawScene();
}

void ToonMapping::Draw()
{
	mPostEffect->Draw();
}

void ToonMapping::DrawDebugGui()
{
	if (Gui::DrawCollapsingHeader("RadialBlur"))
	{
		Gui::DrawSlider1("Gain", mData.gain, 0.01f);
		Gui::DrawSlider1("Offset", mData.offset, 0.01f);
	}
}
