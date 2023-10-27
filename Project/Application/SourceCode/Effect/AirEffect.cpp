#include "AirEffect.h"
using namespace StructuredBufferData;
using namespace ParticleParameter;

AirEffect::AirEffect() :
	mEmitter(std::make_unique<GPUEmitter>())
{
	mEmitter->SetTexture(TextureManager::GetTexture("Particle2"));
	mEmitter->SetParticleData<AirParticle>(100000);
	mEmitter->AddCSStructuredBuffer<SAirEffect>();

	mEffectType = EffectType::AirEffect;
}

void AirEffect::Generate(const Vec3 pos)
{
	mIsActive = true;
	mEmitter->pos = pos;

	mEmitter->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("AirEffect"));
	mEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("AirEffectInit"));
	mEmitter->ExecuteCS();
	mEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("AirEffectUpdate"));
}

void AirEffect::Update()
{
	mEmitter->Update();
}

void AirEffect::Draw()
{
	mEmitter->ExecuteCS();
	mEmitter->Draw();
}
