#include "AttackExplosionEffect.h"
using namespace StructuredBufferData;
using namespace ParticleParameter;

AttackExplosionEffect::AttackExplosionEffect() :
	mEmitter(std::make_unique<GPUEmitter>())
{
	// 円形のパーティクル
	mEmitter->SetTexture(TextureManager::GetTexture("Particle1"));
	mEmitter->SetParticleData<AirParticle>(5000);

	mEffectType = EffectType::AttackExplosionEffect;
}

void AttackExplosionEffect::Generate(const Vec3 pos)
{
	mIsActive = true;
	mEmitter->pos = pos;

	mEmitter->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("AttackExplosionEffect"));
	mEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("AttackExplosionEffectInit"));
	mEmitter->ExecuteCS();
	mEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("AttackExplosionEffectUpdate"));

	mActiveTimer.SetLimitTimer(120);
}

void AttackExplosionEffect::Update()
{
	mActiveTimer.Update();
	if (mActiveTimer == true)
	{
		mIsActive = false;
	}

	mEmitter->Update();
}

void AttackExplosionEffect::Draw()
{
	mEmitter->ExecuteCS();
	mEmitter->Draw("Object3D");
}
