#include "EffectManager.h"

EffectManager::EffectManager() :
	mBloodSprayEffect(std::make_unique<BloodSprayEffect>()),
	mPlayerRecoveryEffect(std::make_unique<PlayerRecoveryEffect>()),
	mRespawnPointEffect(std::make_unique<RespawnPointEffect>()),
	mLeadEffect(std::make_unique<LeadEffect>()),
	mBloom(std::make_unique<Bloom>())
{
}

void EffectManager::Init()
{
	mBloodSprayEffect->Init();
	mPlayerRecoveryEffect->Init();
	mRespawnPointEffect->Init();
	mLeadEffect->Init();
}

void EffectManager::Update()
{
	mBloodSprayEffect->Update();
	mPlayerRecoveryEffect->Update();
	mRespawnPointEffect->Update();
	mLeadEffect->Update();

	mBloom->Update();
}

void EffectManager::RenderTextureSetting()
{
	mBloom->PrevSceneDraw(Bloom::PassType::HighLumi);
	// ���X�|�[���n�_�̃G�t�F�N�g
	mRespawnPointEffect->DrawModel();

	// ��
	mPlayerRecoveryEffect->DrawModel();

	// �����݂����ȃG�t�F�N�g
	mLeadEffect->DrawModel();
	mBloom->PostSceneDraw(Bloom::PassType::HighLumi);

	// ���X�|�[���n�_�̃G�t�F�N�g
	mBloom->PrevSceneDraw(Bloom::PassType::GaussianBlur);
	mBloom->DrawPass(Bloom::PassType::HighLumi);
	mBloom->PostSceneDraw(Bloom::PassType::GaussianBlur);

	mBloom->PrevSceneDraw(Bloom::PassType::Bloom);
	mBloom->DrawPass(Bloom::PassType::GaussianBlur);
	mBloom->PostSceneDraw(Bloom::PassType::Bloom);

	//mBloom->PrevSceneDraw(Bloom::PassType::Target);
	//mRespawnPointEffect->DrawModel();
	//mBloom->PostSceneDraw(Bloom::PassType::Target);


	//// ���X�|�[���n�_�̃G�t�F�N�g
	//mBloom->PrevSceneDraw(Bloom::PassType::HighLumi2);
	//mBloom->DrawPass(Bloom::PassType::HighLumi);
	//mBloom->PostSceneDraw(Bloom::PassType::HighLumi2);

	//// ���X�|�[���n�_�̃G�t�F�N�g
	//mBloom->PrevSceneDraw(Bloom::PassType::GaussianBlur2);
	//mBloom->DrawPass(Bloom::PassType::GaussianBlur);
	//mBloom->PostSceneDraw(Bloom::PassType::GaussianBlur2);
}

void EffectManager::DrawBloom()
{
	mBloom->DrawPostEffect();
}

void EffectManager::DrawModel()
{
	//mBloom->DrawPostEffect();

	// ��
	mBloodSprayEffect->DrawModel();

	// ��
	mPlayerRecoveryEffect->DrawModel();

	// ���X�|�[���n�_�̃G�t�F�N�g
	mRespawnPointEffect->DrawModel();

	// �����݂����ȃG�t�F�N�g
	mLeadEffect->DrawModel();
}

void EffectManager::GenerateBloodSprayEffect(const Vec3 pos)
{
	mBloodSprayEffect->Generate(pos);
}

void EffectManager::GeneratePlayerRecoveryEffect(const Vec3 pos)
{
	mPlayerRecoveryEffect->Generate(pos);
}

void EffectManager::GenerateRespawnPointEffect(const Vec3 pos)
{
	mRespawnPointEffect->Generate(pos);
}

void EffectManager::GenerateLeadEffect(const Vec3 pos, const Vec3 frontVec)
{
	mLeadEffect->Generate(pos, frontVec);
}

Bloom* EffectManager::GetBloom()
{
	return mBloom.get();
}
