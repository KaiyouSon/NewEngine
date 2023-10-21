#pragma once
#include "BloodSprayEffect.h"
#include "PlayerRecoveryEffect.h"
#include "RespawnPointEffect.h"
#include "LeadEffect.h"
#include "AirEffect.h"
#include "LogoExplosionEffect.h"
#include "Player.h"

template<typename T> class Singleton;

class EffectManager : public Singleton<EffectManager>
{

private:
	std::vector<std::unique_ptr<IEffect>> mEffects;

	std::unique_ptr<BloodSprayEffect> mBloodSprayEffect;
	std::unique_ptr<RespawnPointEffect> mRespawnPointEffect;

private:
	Player* mPlayer;

public:
	EffectManager();
	void Init();
	void Update();
	void DrawModel();
	void DrawEffect(const bool isBloom = false);

public:
	void GenerateAirEffect(const Vec3 pos);
	void GenerateBloodSprayEffect(const Vec3 pos);
	void GeneratePlayerRecoveryEffect(const Vec3 pos);
	void GenerateRespawnPointEffect(const Vec3 pos);
	void GenerateLeadEffect(const Vec3 pos, const Vec3 frontVec);
	void GenerateLogoExplosionEffect(const Vec3 pos, const Vec3 rot, const Vec3 scale);

public:
	void ExplosionLogoExplosionEffect();

public:
	void SetPlayer(Player* player);

private:
	friend Singleton<EffectManager>;
};
