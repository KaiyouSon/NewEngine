#include "EffectManager.h"

EffectManager::EffectManager() :
	mBloodSprayEffect(std::make_unique<BloodSprayEffect>()),
	mPlayerRecoveryEffect(std::make_unique<PlayerRecoveryEffect>()),
	mRespawnPointEffect(std::make_unique<RespawnPointEffect>()),
	mLeadEffect(std::make_unique<LeadEffect>())
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

void EffectManager::DrawEffect(const bool isBloom)
{
	// �u���[�����ʂ��������G�t�F�N�g
	if (isBloom == true)
	{
		// ��
		mPlayerRecoveryEffect->DrawModel();

		// ���X�|�[���n�_�̃G�t�F�N�g
		mRespawnPointEffect->DrawModel();

		// �����݂����ȃG�t�F�N�g
		mLeadEffect->DrawModel();
	}
	// ����ȊO
	else
	{
		// ��
		mBloodSprayEffect->DrawModel();

		// ��
		mPlayerRecoveryEffect->DrawModel();

		// ���X�|�[���n�_�̃G�t�F�N�g
		mRespawnPointEffect->DrawModel();

		// �����݂����ȃG�t�F�N�g
		mLeadEffect->DrawModel();
	}
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

void EffectManager::SetPlayer(Player* player)
{
	mPlayer = player;
}
