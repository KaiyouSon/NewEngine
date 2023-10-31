#include "PlayerRecoveryEffect.h"
using namespace StructuredBufferData;
using namespace ConstantBufferData;
using namespace ParticleParameter;

PlayerRecoveryEffect::PlayerRecoveryEffect() :
	mCircleEmitter(std::make_unique<GPUEmitter>()),
	mLineEmitter(std::make_unique<GPUEmitter>())
{
	// 円形のパーティクル
	mCircleEmitter->SetTexture(TextureManager::GetTexture("Particle1"));
	mCircleEmitter->SetParticleData<PlayerRecoveryParticle>(5000);

	mLineEmitter->SetTexture(TextureManager::GetTexture("Line"));
	mLineEmitter->SetParticleData<PlayerRecoveryParticle>(100);

	mEffectType = EffectType::PlayerRecoveryEffect;
}

void PlayerRecoveryEffect::Generate(const Vec3 pos)
{
	mIsActive = true;

	mCircleEmitter->pos = pos;
	mLineEmitter->pos = pos;

	mCircleEmitter->AddCSConstantBuffer<CPlayerRecoveryEffect>();
	mLineEmitter->AddCSConstantBuffer<CPlayerRecoveryEffect>();

	// --- 円形のパーティクル ----------------------------------------------- //

	// スケール
	mCircleEmitterParam.baseScale = 0.05f;
	mCircleEmitterParam.rateScale = 0.15f;

	// スケール変化速度
	mCircleEmitterParam.baseScaleAccel = 0.00125f;
	mCircleEmitterParam.rateScaleAccel = 0.00125f;

	// 速度
	mCircleEmitterParam.baseSpeed = Vec3(0.05f, 0.05f, 0.05f);
	mCircleEmitterParam.rateSpeed = mCircleEmitterParam.baseSpeed * 2;
	mCircleEmitterParam.rateSpeed.y = 0;

	// 色
	mCircleEmitterParam.baseColor = Color(0xffa80b).To01();

	// 円形のパーティクル
	mCircleEmitter->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("PlayerRecoveryEffect"));
	mCircleEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("PlayerRecoveryEffectInit"));
	mCircleEmitter->TransferCSConstantBuffer(1, mCircleEmitterParam);
	mCircleEmitter->ExecuteCS();
	mCircleEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("PlayerRecoveryEffectUpdate"));


	// --- 線のパーティクル ------------------------------------------------- //

	// スケール
	mLineEmitterParam.baseScale = 1.f;
	mLineEmitterParam.rateScale = 0.f;

	// スケール変化速度
	mLineEmitterParam.baseScaleAccel = 0.005f;
	mLineEmitterParam.rateScaleAccel = 0.005f;

	// 速度
	mLineEmitterParam.baseSpeed.y = 0.05f;
	mLineEmitterParam.rateSpeed = 0;

	// 色
	mLineEmitterParam.baseColor = Color(0xffe95c).To01();

	mLineEmitter->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("PlayerRecoveryEffect"));
	mLineEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("PlayerRecoveryEffectInit"));
	mLineEmitter->TransferCSConstantBuffer(1, mLineEmitterParam);
	mLineEmitter->ExecuteCS();
	mLineEmitter->SetComputePipeline(PipelineManager::GetComputePipeline("PlayerRecoveryEffectUpdate"));

	mActiveTimer.SetLimitTimer(120);
}

void PlayerRecoveryEffect::Update()
{
	mCircleEmitter->rot.y += Radian(1);

	mActiveTimer.Update();
	if (mActiveTimer == true)
	{
		mIsActive = false;
	}

	mCircleEmitter->Update();
	mLineEmitter->Update();
}

void PlayerRecoveryEffect::Draw()
{
	mCircleEmitter->ExecuteCS();
	mLineEmitter->ExecuteCS();

	mCircleEmitter->Draw();
	mLineEmitter->Draw();
}