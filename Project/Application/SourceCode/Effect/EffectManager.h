#pragma once
#include "BloodSprayEffect.h"
#include "PlayerRecoveryEffect.h"
#include "RespawnPointEffect.h"
#include "LeadEffect.h"
#include "AirEffect.h"
#include "BossAttack1Effect.h"
#include "Player.h"

template<typename T> class Singleton;

class EffectManager : public Singleton<EffectManager>
{
private:
	std::unique_ptr<BloodSprayEffect> mBloodSprayEffect;
	std::unique_ptr<PlayerRecoveryEffect> mPlayerRecoveryEffect;
	std::unique_ptr<RespawnPointEffect> mRespawnPointEffect;
	std::unique_ptr<AirEffect> mAirEffect;

	std::vector<std::unique_ptr<LeadEffect>> mLeadEffect;
	std::vector<std::unique_ptr<BossAttack1Effect>> mBossAttack1Effect;

private:
	Player* mPlayer;

public:
	EffectManager();
	void Init();
	void Update();
	void DrawModel();
	void DrawEffect(const bool isBloom = false);

public:
	void GenerateBloodSprayEffect(const Vec3 pos);
	void GeneratePlayerRecoveryEffect(const Vec3 pos);
	void GenerateRespawnPointEffect(const Vec3 pos);
	void GenerateLeadEffect(const Vec3 pos, const Vec3 frontVec);
	void GenerateBossAttack1Effect(const Vec3 pos);

public:
	void SetPlayer(Player* player);

private:
	friend Singleton<EffectManager>;
};
