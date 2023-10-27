#include "EffectManager.h"

EffectManager::EffectManager()
{
}

void EffectManager::Init()
{
	// インスタンス生成
	mBloodSprayEffect = std::make_unique<BloodSprayEffect>();
	//mRespawnPointEffect = std::make_unique<RespawnPointEffect>();

	// 初期化
	mBloodSprayEffect->Init();
	//mRespawnPointEffect->Init();

	mEffects.clear();
}

void EffectManager::Update()
{
	if (Key::GetKeyDown(DIK_G))
	{
		GenerateAirEffect(Vec3::up * 10.f);
	}

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
}

void EffectManager::DrawModel()
{
	// 陦
	mBloodSprayEffect->DrawModel();
}

void EffectManager::DrawEffect(const bool isBloom)
{
	// ブルームかけるやつ
	if (isBloom == true)
	{
		//mRespawnPointEffect->DrawModel();

		for (uint32_t i = 0; i < mEffects.size(); i++)
		{
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
	}
}

void EffectManager::GenerateAirEffect(const Vec3 pos)
{
	// インスタンス生成
	std::unique_ptr<AirEffect> effect = std::make_unique<AirEffect>();
	effect->Generate(pos);

	// ベクターに追加
	mEffects.push_back(std::move(effect));
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

void EffectManager::GenerateLogoExplosionEffect(const Vec3 pos, const Vec3 rot, const Vec3 scale)
{
	// インスタンス生成
	std::unique_ptr<LogoExplosionEffect> effect = std::make_unique<LogoExplosionEffect>();
	effect->Generate(pos, rot, scale);

	// ベクターに追加
	mEffects.push_back(std::move(effect));
}

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

void EffectManager::SetPlayer(Player* player)
{
	mPlayer = player;
}

