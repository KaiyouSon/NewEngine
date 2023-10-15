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
	if (Key::GetKeyDown(DIK_G))
	{
		GenerateBossAttack1Effect(Vec3::zero);
	}
	if (Key::GetKeyDown(DIK_C))
	{
		mBossAttack1Effect.clear();
	}

	mBloodSprayEffect->Update();
	mPlayerRecoveryEffect->Update();
	mRespawnPointEffect->Update();
	mLeadEffect->Update();
	mAirEffect->Update();

	for (uint32_t i = 0; i < mBossAttack1Effect.size(); i++)
	{
		mBossAttack1Effect[i]->Update();
	}

	mAirEffect->Generate(mPlayer->GetPos());
}

void EffectManager::DrawModel()
{
	// 陦
	mBloodSprayEffect->DrawModel();

	// 蝗槫ｾｩ
	mPlayerRecoveryEffect->DrawModel();

	// 繝ｪ繧ｹ繝昴・繝ｳ蝨ｰ轤ｹ縺ｮ繧ｨ繝輔ぉ繧ｯ繝・
	mRespawnPointEffect->DrawModel();

	// 蟆手勠縺ｿ縺溘＞縺ｪ繧ｨ繝輔ぉ繧ｯ繝・
	mLeadEffect->DrawModel();
}

void EffectManager::DrawEffect(const bool isBloom)
{
	// 繝悶Ν繝ｼ繝蜉ｹ譫懊°縺代◆縺・お繝輔ぉ繧ｯ繝・
	if (isBloom == true)
	{
		//// 蝗槫ｾｩ
		mPlayerRecoveryEffect->DrawModel();

		// 繝ｪ繧ｹ繝昴・繝ｳ蝨ｰ轤ｹ縺ｮ繧ｨ繝輔ぉ繧ｯ繝・
		mRespawnPointEffect->DrawModel();

		//// 蟆手勠縺ｿ縺溘＞縺ｪ繧ｨ繝輔ぉ繧ｯ繝・
		mLeadEffect->DrawModel();

		//// 遨ｺ荳ｭ縺ｫ縺ゅｋ繧・▽
		mAirEffect->DrawModel();

		for (uint32_t i = 0; i < mBossAttack1Effect.size(); i++)
		{
			mBossAttack1Effect[i]->Draw();
		}
	}
	// 縺昴ｌ莉･螟・
	else
	{
		//// 陦
		mBloodSprayEffect->DrawModel();

		//// 蝗槫ｾｩ
		mPlayerRecoveryEffect->DrawModel();

		// 繝ｪ繧ｹ繝昴・繝ｳ蝨ｰ轤ｹ縺ｮ繧ｨ繝輔ぉ繧ｯ繝・
		mRespawnPointEffect->DrawModel();

		//// 蟆手勠縺ｿ縺溘＞縺ｪ繧ｨ繝輔ぉ繧ｯ繝・
		mLeadEffect->DrawModel();

		//// 遨ｺ荳ｭ縺ｫ縺ゅｋ繧・▽
		mAirEffect->DrawModel();

		for (uint32_t i = 0; i < mBossAttack1Effect.size(); i++)
		{
			mBossAttack1Effect[i]->Draw();
		}
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

void EffectManager::GenerateBossAttack1Effect(const Vec3 pos)
{
	mBossAttack1Effect.push_back(std::move(std::make_unique<BossAttack1Effect>()));
	mBossAttack1Effect.back()->Generate(pos);
}

void EffectManager::SetPlayer(Player* player)
{
	mPlayer = player;
}

