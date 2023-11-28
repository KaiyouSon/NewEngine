#include "SmokeEffect.h"
using namespace StructuredBufferData;
using namespace ConstantBufferData;
using namespace ParticleParameter;

SmokeEffect::SmokeEffect() :
	mEmitter(std::make_unique<GPUEmitter>())
{
	mEmitter->SetTexture(TextureManager::GetTexture("SmokeParticle"));
	mEmitter->SetParticleData<SmokeParticle>(512);
	mEmitter->AddCSConstantBuffer<CSmokeEffect>();
	mEmitter->AddCSStructuredBuffer<STimer>();

	mEffectType = EffectType::SmokeEffect;
}

void SmokeEffect::Generate()
{
	mIsActive = true;

	mEmitter->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("SmokeEffect"));
	mEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("SmokeEffectInit"));

	mEmitter->TransferCSConstantBuffer(1, mData);

	mEmitter->ExecuteCS();
	mEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("SmokeEffectUpdate"));
}

void SmokeEffect::Update()
{
	mEmitter->TransferCSConstantBuffer(1, mData);

	mEmitter->Update();
}

void SmokeEffect::ExecuteCS()
{
	mEmitter->ExecuteCS();
}

void SmokeEffect::Draw()
{
	mEmitter->Draw("Object3D");
}

void SmokeEffect::Execute(const CSmokeEffect data)
{
	mData = data;
}
