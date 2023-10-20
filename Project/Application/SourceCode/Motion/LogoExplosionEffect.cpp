#include "LogoExplosionEffect.h"

LogoExplosionEffect::LogoExplosionEffect() :
	mParticleMesh(std::make_unique<ParticleMesh>())
{
	mParticleMesh->SetMeshTexture(TextureManager::GetTexture("TitleLogo"));
	mParticleMesh->SetParticleTexture(TextureManager::GetTexture("Particle1"));
	mParticleMesh->SetParticleData<ParticleParameter::BossAttack1Particle>(100000);

	mParticleMesh->SetComputePipeline(PipelineManager::GetComputePipeline("LogoExplosionEffectInit"));
	mParticleMesh->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("LogoExplosionEffect"));
	mParticleMesh->ExecuteCS();
	mParticleMesh->SetComputePipeline(PipelineManager::GetComputePipeline("LogoExplosionEffectUpdate"));

	mEffectType = EffectType::LogoExplosionEffect;
}

void LogoExplosionEffect::Generate(const Vec3 pos, const Vec3 rot, const Vec3 scale)
{
	mParticleMesh->pos = pos;
	mParticleMesh->rot = rot;
	mParticleMesh->scale = scale;

	mActiveTimer.SetLimitTimer(240);
	mIsActive = true;
	mIsExplosion = false;
}

void LogoExplosionEffect::Update()
{
	if (mIsExplosion == true)
	{
		mActiveTimer.Update();
		if (mActiveTimer == true)
		{
			mIsActive = false;
		}
	}

	mParticleMesh->Update();
}

void LogoExplosionEffect::Draw()
{
	if (mIsExplosion == true)
	{
		mParticleMesh->ExecuteCS();
	}

	mParticleMesh->Draw();
}

void LogoExplosionEffect::SetisExplosion(const bool isExplosion)
{
	mIsExplosion = isExplosion;
}
