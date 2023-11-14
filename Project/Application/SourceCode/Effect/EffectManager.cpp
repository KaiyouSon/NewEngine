#include "EffectManager.h"
#include "PlayerRecoveryEffect.h"
#include "LeadEffect.h"
#include "AirEffect.h"
#include "LogoExplosionEffect.h"
#include "BossAttackTrajectoryEffect.h"
#include "AttackExplosionEffect.h"
#include "SmokeEffect.h"

EffectManager::EffectManager()
{
}

void EffectManager::Init()
{
	// インスタンス生成
	mBloodSprayEffect = std::make_unique<BloodSprayEffect>();

	// 初期化
	mBloodSprayEffect->Init();

	mEffects.clear();

	// 空中のエフェクトのエミッタ生成
	GenerateAirEffect(Vec3::up * 10.f);

	// ボスの攻撃のエミッタ生成
	GenerateBossAttackTrajectoryEffect();

	// 煙のエミッタ生成
	for (uint32_t i = 0; i < (uint32_t)SmokeEffectIndex::Size; i++)
	{
		GenerateSmokeEffect();
	}
}
void EffectManager::Update()
{
	// デバッグのみ実行
	ProcessAtDebugBuild([this]
		{
			if (Key::GetKeyDown(DIK_G))
			{
				GenerateAttackExplosionEffect(Vec3::up * 10.f);

				//GenerateBossAttackTrajectoryEffect();
			}
		});


	mBloodSprayEffect->Update();

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

	for (uint32_t i = 0; i < mSmokeEffects.size(); i++)
	{
		mSmokeEffects[i]->Update();
	}
}
void EffectManager::DrawEffect(const bool isBloom)
{
	// ブルームかけるやつ
	if (isBloom == true)
	{
		for (uint32_t i = 0; i < mEffects.size(); i++)
		{
			// 煙エフェクトにはBloomかけない
			if (mEffects[i]->GetEffectType() == EffectType::SmokeEffect)
			{
				continue;
			}

			mEffects[i]->Draw();
		}
	}
	// ブルームかけたくないやつ
	else
	{
		// 血
		mBloodSprayEffect->DrawModel();

		for (uint32_t i = 0; i < mEffects.size(); i++)
		{
			mEffects[i]->Draw();
		}

		for (uint32_t i = 0; i < mSmokeEffects.size(); i++)
		{
			mSmokeEffects[i]->Draw();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --- 生成 --------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 空中のエフェクト
void EffectManager::GenerateAirEffect(const Vec3 pos)
{
	// インスタンス生成
	std::unique_ptr<AirEffect> effect = std::make_unique<AirEffect>();
	effect->Generate(pos);

	// ベクターに追加
	mEffects.push_back(std::move(effect));
}

// 攻撃後のエフェクト
void EffectManager::GenerateAttackExplosionEffect(const Vec3 pos)
{
	// インスタンス生成
	std::unique_ptr<AttackExplosionEffect> effect = std::make_unique<AttackExplosionEffect>();
	effect->Generate(pos);

	// ベクターに追加
	mEffects.push_back(std::move(effect));
}

// 血のエフェクト
void EffectManager::GenerateBloodSprayEffect(const Vec3 pos)
{
	mBloodSprayEffect->Generate(pos);
}

// プレイヤーの回復のエフェクト
void EffectManager::GeneratePlayerRecoveryEffect(const Vec3 pos)
{
	// インスタンス生成
	std::unique_ptr<PlayerRecoveryEffect> effect = std::make_unique<PlayerRecoveryEffect>();
	effect->Generate(pos);

	// ベクターに追加
	mEffects.push_back(std::move(effect));
}

// 誘導エフェクト
void EffectManager::GenerateLeadEffect(const Vec3 pos, const Vec3 frontVec)
{
	// インスタンス生成
	std::unique_ptr<LeadEffect> effect = std::make_unique<LeadEffect>();
	effect->Generate(pos, frontVec);

	// ベクターに追加
	mEffects.push_back(std::move(effect));
}

// タイトルロゴが爆散するエフェクト
void EffectManager::GenerateLogoExplosionEffect(const Vec3 pos, const Vec3 rot, const Vec3 scale)
{
	// インスタンス生成
	std::unique_ptr<LogoExplosionEffect> effect = std::make_unique<LogoExplosionEffect>();
	effect->Generate(pos, rot, scale);

	// ベクターに追加
	mEffects.push_back(std::move(effect));
}

// ボスの攻撃の軌跡のエフェクト
void EffectManager::GenerateBossAttackTrajectoryEffect()
{
	// インスタンス生成
	std::unique_ptr<BossAttackTrajectoryEffect> effect = std::make_unique<BossAttackTrajectoryEffect>();
	effect->Generate();

	// ベクターに追加
	mEffects.push_back(std::move(effect));
}

// 煙のエフェクト
void EffectManager::GenerateSmokeEffect()
{
	// インスタンス生成
	std::unique_ptr<SmokeEffect> effect = std::make_unique<SmokeEffect>();
	effect->Generate();

	// ベクターに追加
	mSmokeEffects.push_back(std::move(effect));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --- 実行 --------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// タイトルロゴが爆散するエフェクト
void EffectManager::ExplosionLogoExplosionEffect()
{
	for (uint32_t i = 0; i < mEffects.size(); i++)
	{
		// 先に格納したエフェクトから爆散する
		if (mEffects[i]->GetEffectType() == EffectType::LogoExplosionEffect)
		{
			LogoExplosionEffect* effect = dynamic_cast<LogoExplosionEffect*>(mEffects[i].get());
			effect->SetisExplosion(true);
			break;
		}
	}
}

// ボスの攻撃の軌跡のエフェクト
void EffectManager::ExecuteBossAttackTrajectoryEffect(const bool isGenerate, const Vec3 startPos, const Vec3 endPos)
{
	for (uint32_t i = 0; i < mEffects.size(); i++)
	{
		// 先に格納したエフェクト
		if (mEffects[i]->GetEffectType() == EffectType::BossAttackTrajectoryEffect)
		{
			BossAttackTrajectoryEffect* effect = dynamic_cast<BossAttackTrajectoryEffect*>(mEffects[i].get());
			effect->Execute(isGenerate, startPos, endPos);
			break;
		}
	}
}

// 煙のエフェクト
void EffectManager::ExecuteSmokeEffect(const ConstantBufferData::CSmokeEffect data,const SmokeEffectIndex index)
{
	SmokeEffect* effect = dynamic_cast<SmokeEffect*>(mSmokeEffects[(uint32_t)index].get());
	effect->Execute(data);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --- セッター ----------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void EffectManager::SetPlayer(Player* player)
{
	mPlayer = player;
}

