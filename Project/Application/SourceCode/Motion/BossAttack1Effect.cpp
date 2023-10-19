#include "BossAttack1Effect.h"

BossAttack1Effect::BossAttack1Effect()
{
	mParticleMesh = std::make_unique<ParticleMesh>();
	mParticleMesh->SetMeshTexture(TextureManager::GetTexture("TitleLogo"));
	mParticleMesh->SetParticleTexture(TextureManager::GetTexture("Particle1"));
	mParticleMesh->SetParticleData<ParticleParameter::BossAttack1Particle>(100000);

	mParticleMesh->SetComputePipeline(PipelineManager::GetComputePipeline("BossAttack1EffectInit"));
	mParticleMesh->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("BossAttack1Effect"));
	mParticleMesh->ExecuteComputeShader();
}

void BossAttack1Effect::Generate(const Vec3 pos)
{
	mParticleMesh->pos = pos;
	mParticleMesh->scale /= 10.f;

	mParticleMesh->SetComputePipeline(PipelineManager::GetComputePipeline("BossAttack1EffectUpdate"));

	mLifeTimer.SetLimitTimer(120);
	mIsActive = true;
}

void BossAttack1Effect::Update()
{
	mLifeTimer.Update();
	if (mLifeTimer == true)
	{
		mIsActive = false;
	}

	//mParticleMesh->rot.y += Radian(1);
	mParticleMesh->Update();
}

void BossAttack1Effect::Draw()
{
	mParticleMesh->ExecuteComputeShader();
	mParticleMesh->Draw();
}

bool BossAttack1Effect::GetisActive()
{
	return mIsActive;
}
