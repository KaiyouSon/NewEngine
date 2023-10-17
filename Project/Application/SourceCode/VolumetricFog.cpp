#include "VolumetricFog.h"

using namespace StructuredBufferData;

VolumetricFog::VolumetricFog() :
	mPostEffect(std::make_unique<PostEffect>())
{
	mRenderTexture = TextureManager::GetRenderTexture("VolumetricFog");

	mPostEffect->pos = GetWindowHalfSize();
	mPostEffect->AddRenderTexture(mRenderTexture);
	mPostEffect->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("VolumetricFog"));
	mPostEffect->SetComputePipeline(PipelineManager::GetComputePipeline("VolumetricFog"));
	mPostEffect->AddSBToMaterial<SFog>(static_cast<uint32_t>(GetWindowSize().x * GetWindowSize().y));
}

void VolumetricFog::Update()
{
	mPostEffect->Update();
}

void VolumetricFog::PrevDrawScene()
{
	mRenderTexture->PrevDrawScene();
}

void VolumetricFog::PostDrawScene()
{
	mRenderTexture->PostDrawScene();
}

void VolumetricFog::Draw()
{
	mPostEffect->ExecuteCS(2);
	mPostEffect->Draw();
}
