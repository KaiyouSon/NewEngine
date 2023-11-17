#include "AirEffect.h"
using namespace StructuredBufferData;
using namespace ConstantBufferData;
using namespace ParticleParameter;

AirEffect::AirEffect() :
	mEmitter(std::make_unique<GPUEmitter>())
{
	mEmitter->SetTexture(TextureManager::GetTexture("Particle2"));
	mEmitter->SetParticleData<AirParticle>(102400);
	mEmitter->AddCSStructuredBuffer<STimer>();
	mEmitter->AddCSConstantBuffer<Vec3>();

	mEffectType = EffectType::AirEffect;
}

void AirEffect::Generate(const Vec3 pos)
{
	mIsActive = true;
	mGeneratePos = pos;

	mEmitter->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("AirEffect"));
	mEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("AirEffectInit"));

	mEmitter->TransferCSConstantBuffer(1, mGeneratePos);

	mEmitter->ExecuteCS();
	mEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("AirEffectUpdate"));
}

void AirEffect::Update()
{
	mEmitter->TransferCSConstantBuffer(1, mGeneratePos);
	mEmitter->Update();
}

void AirEffect::Draw()
{
	mEmitter->ExecuteCS();
	mEmitter->Draw();
}

void AirEffect::Execute(const Vec3 pos)
{
	mGeneratePos = pos;
}
