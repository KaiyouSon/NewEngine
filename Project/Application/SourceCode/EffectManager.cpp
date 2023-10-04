#include "EffectManager.h"

EffectManager::EffectManager() :
	mBloodSprayEffect(std::make_unique<BloodSprayEffect>()),
	mPlayerRecoveryEffect(std::make_unique<PlayerRecoveryEffect>()),
	mRespawnPointEffect(std::make_unique<RespawnPointEffect>()),
	mLeadEffect(std::make_unique<LeadEffect>()),
	mAirEffect(std::make_unique<AirEffect>())
{
}

void EffectManager::Init()
{
	mBloodSprayEffect->Init();
	mPlayerRecoveryEffect->Init();
	mRespawnPointEffect->Init();
	mLeadEffect->Init();
	mAirEffect->Init();
}

void EffectManager::Update()
{
	mBloodSprayEffect->Update();
	mPlayerRecoveryEffect->Update();
	mRespawnPointEffect->Update();
	mLeadEffect->Update();
	mAirEffect->Update();

	mAirEffect->Generate(mPlayer->GetPos());
}

void EffectManager::DrawModel()
{
	//mBloom->DrawPostEffect();

	// 血
	mBloodSprayEffect->DrawModel();

	// 回復
	mPlayerRecoveryEffect->DrawModel();

	// リスポーン地点のエフェクト
	mRespawnPointEffect->DrawModel();

	// 導虫みたいなエフェクト
	mLeadEffect->DrawModel();
}

void EffectManager::DrawEffect(const bool isBloom)
{
	// ブルーム効果かけたいエフェクト
	if (isBloom == true)
	{
		//// 回復
		//mPlayerRecoveryEffect->DrawModel();

		// リスポーン地点のエフェクト
		mRespawnPointEffect->DrawModel();

		//// 導虫みたいなエフェクト
		//mLeadEffect->DrawModel();

		//// 空中にあるやつ
		//mAirEffect->DrawModel();
	}
	// それ以外
	else
	{
		//// 血
		//mBloodSprayEffect->DrawModel();

		//// 回復
		//mPlayerRecoveryEffect->DrawModel();

		// リスポーン地点のエフェクト
		mRespawnPointEffect->DrawModel();

		//// 導虫みたいなエフェクト
		//mLeadEffect->DrawModel();

		//// 空中にあるやつ
		//mAirEffect->DrawModel();
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
