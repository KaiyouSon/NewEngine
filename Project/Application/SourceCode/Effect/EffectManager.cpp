#include "EffectManager.h"

EffectManager::EffectManager()
{
}

void EffectManager::Init()
{
	// インスタンス生成
	mBloodSprayEffect = std::make_unique<BloodSprayEffect>();
	mPlayerRecoveryEffect = std::make_unique<PlayerRecoveryEffect>();
	//mRespawnPointEffect = std::make_unique<RespawnPointEffect>();

	mAirEffect = std::make_unique<AirEffect>();

	// 初期化
	mBloodSprayEffect->Init();
	//mRespawnPointEffect->Init();
	mAirEffect->Init();

	mEffects.clear();
}

void EffectManager::Update()
{
	if (Key::GetKeyDown(DIK_G))
	{
		GeneratePlayerRecoveryEffect(Vec3::up * 10.f);
		//GenerateBossAttack1Effect(Vec3::zero);

		//GenerateLeadEffect(Vec3::up * 10.f, Vec3::front);
	}
	if (Key::GetKeyDown(DIK_C))
	{
		mBossAttack1Effect.clear();
	}

	mBloodSprayEffect->Update();
	//mPlayerRecoveryEffect->Update();
	//mRespawnPointEffect->Update();
	mAirEffect->Update();

	std::erase_if(mBossAttack1Effect,
		[](const std::unique_ptr<BossAttack1Effect>& emitter)
		{
			return emitter->GetisActive() == false;
		});

	for (uint32_t i = 0; i < mBossAttack1Effect.size(); i++)
	{
		mBossAttack1Effect[i]->Update();
	}

	// 削除処理
	std::erase_if(mEffects,
		[](const std::unique_ptr<IEffect>& effect)
		{
			return effect->GetisActive() == false;
		});

	for (uint32_t i = 0; i < mEffects.size(); i++)
	{
		mEffects[i]->Update();
	}

	mAirEffect->Generate(mPlayer->GetPos());
}

void EffectManager::DrawModel()
{
	// 陦
	mBloodSprayEffect->DrawModel();

	// 蝗槫ｾｩ
	//mPlayerRecoveryEffect->DrawModel();

	// 繝ｪ繧ｹ繝昴・繝ｳ蝨ｰ轤ｹ縺ｮ繧ｨ繝輔ぉ繧ｯ繝・
	//mRespawnPointEffect->DrawModel();

	// 蟆手勠縺ｿ縺溘＞縺ｪ繧ｨ繝輔ぉ繧ｯ繝・
}

void EffectManager::DrawEffect(const bool isBloom)
{
	// 繝悶Ν繝ｼ繝蜉ｹ譫懊°縺代◆縺・お繝輔ぉ繧ｯ繝・
	if (isBloom == true)
	{
		//// 蝗槫ｾｩ
		//mPlayerRecoveryEffect->DrawModel();

		// 繝ｪ繧ｹ繝昴・繝ｳ蝨ｰ轤ｹ縺ｮ繧ｨ繝輔ぉ繧ｯ繝・
		//mRespawnPointEffect->DrawModel();

		//// 遨ｺ荳ｭ縺ｫ縺ゅｋ繧・▽
		mAirEffect->DrawModel();

		for (uint32_t i = 0; i < mBossAttack1Effect.size(); i++)
		{
			mBossAttack1Effect[i]->Draw();
		}

		for (uint32_t i = 0; i < mEffects.size(); i++)
		{
			mEffects[i]->Draw();
		}
	}
	// 縺昴ｌ莉･螟・
	else
	{
		//// 陦
		mBloodSprayEffect->DrawModel();

		//// 蝗槫ｾｩ
		//mPlayerRecoveryEffect->DrawModel();

		// 繝ｪ繧ｹ繝昴・繝ｳ蝨ｰ轤ｹ縺ｮ繧ｨ繝輔ぉ繧ｯ繝・

		//// 蟆手勠縺ｿ縺溘＞縺ｪ繧ｨ繝輔ぉ繧ｯ繝・

		//// 遨ｺ荳ｭ縺ｫ縺ゅｋ繧・▽
		mAirEffect->DrawModel();

		for (uint32_t i = 0; i < mEffects.size(); i++)
		{
			mEffects[i]->Draw();
		}

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
	// インスタンス生成
	std::unique_ptr<PlayerRecoveryEffect> effect = std::make_unique<PlayerRecoveryEffect>();
	effect->Generate(pos);

	// ベクターに追加
	mEffects.push_back(std::move(effect));
}

void EffectManager::GenerateRespawnPointEffect(const Vec3 pos)
{
	pos;
	//mRespawnPointEffect->Generate(pos);
}

void EffectManager::GenerateLeadEffect(const Vec3 pos, const Vec3 frontVec)
{
	// インスタンス生成
	std::unique_ptr<LeadEffect> effect = std::make_unique<LeadEffect>();
	effect->Generate(pos, frontVec);

	// ベクターに追加
	mEffects.push_back(std::move(effect));
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

