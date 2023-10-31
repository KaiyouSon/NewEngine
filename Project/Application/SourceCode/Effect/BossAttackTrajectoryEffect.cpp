#include "BossAttackTrajectoryEffect.h"
using namespace StructuredBufferData;
using namespace ParticleParameter;

BossAttackTrajectoryEffect::BossAttackTrajectoryEffect() :
	mEmitter(std::make_unique<GPUEmitter>())
{
	mEmitter->SetTexture(TextureManager::GetTexture("Particle2"));
	mEmitter->SetParticleData<BossAttackTrajectoryParticle>(10000);
	mEmitter->AddCSConstantBuffer<Vec3>();
	mEmitter->AddCSStructuredBuffer<SBossAttackTrajectoryEffect>();

	mEffectType = EffectType::BossAttackTrajectoryEffect;
}

void BossAttackTrajectoryEffect::Generate(const Vec3 startPos, const Vec3 endPos)
{
	mEmitter->pos = Vec3::up * 10;

	mIsActive = true;
	mStartPos = startPos;
	mEndPos = endPos;

	mEmitter->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("BossAttackTrajectoryEffect"));
	mEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("BossAttackTrajectoryEffectInit"));

	// ベクトル転送
	Vec3 vec = mEndPos - mStartPos;
	mEmitter->TransferCSConstantBuffer(1, vec);

	mEmitter->ExecuteCS();
	mEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("BossAttackTrajectoryEffectUpdate"));
}

void BossAttackTrajectoryEffect::Update()
{
	// ベクトル転送
	Vec3 vec = mEndPos - mStartPos;
	mEmitter->TransferCSConstantBuffer(1, vec);

	mEmitter->Update();
}

void BossAttackTrajectoryEffect::Draw()
{
	mEmitter->ExecuteCS();
	mEmitter->Draw();
}
