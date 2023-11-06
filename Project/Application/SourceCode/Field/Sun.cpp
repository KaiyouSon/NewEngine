#include "Sun.h"

Sun::Sun() :
	mSun(std::make_unique<ParticleObject>())
{
	mSun->SetParticleTexture(TextureManager::GetTexture("Particle2"));
}

void Sun::Init()
{
	mSun->SetParticleData<ParticleParameter::PonDeRing>(ModelManager::GetModel("PonDeRing"), 1000);
	mSun->SetComputePipeline(PipelineManager::GetComputePipeline("PonDeRingInit"));
	mSun->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("PonDeRing"));
	mSun->ExecuteCS(4160);
	mSun->SetComputePipeline(PipelineManager::GetComputePipeline("PonDeRingUpdate"));
}

void Sun::Update()
{
	mSun->Update();
}

void Sun::Draw()
{
	mSun->ExecuteCS(4160);
	mSun->Draw();
}

void Sun::SetTransform(const Transform& transform)
{
	mSun->pos = transform.pos;
	mSun->scale = transform.scale;
	mSun->rot = transform.rot;
}
