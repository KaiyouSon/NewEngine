#include "LeadEffect.h"
using namespace StructuredBufferData;
using namespace ParticleParameter;

LeadEffect::LeadEffect() :
	mEmitter(std::make_unique<GPUEmitter>())
{
	mEmitter->SetTexture(TextureManager::GetTexture("Particle2"));
	mEmitter->SetParticleData<LeadParticle>(100000);
	mEmitter->AddCSConstantBuffer<Vec3>();
	mEmitter->AddCSStructuredBuffer<SLeadEffect>();

	mEffectType = EffectType::LeadEffect;
}

void LeadEffect::Generate(const Vec3 pos, const Vec3 frontVec)
{
	mIsActive = true;
	mEmitter->pos = pos;
	mVec = frontVec;

	mEmitter->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("LeadEffect"));
	mEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("LeadEffectInit"));
	mEmitter->TransferCSConstantBuffer(1, mVec);
	mEmitter->ExecuteCS();
	mEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("LeadEffectUpdate"));
}

void LeadEffect::Update()
{
	mEmitter->TransferCSConstantBuffer(1, mVec);
	mEmitter->Update();
}

void LeadEffect::Draw()
{
	mEmitter->ExecuteCS();
	mEmitter->Draw();
}
