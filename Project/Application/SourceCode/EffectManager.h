#pragma once
#include "BloodSprayEffect.h"
#include "PlayerRecoveryEffect.h"
#include "RespawnPointEffect.h"
#include "LeadEffect.h"
#include "Bloom.h"
#include "Player.h"

template<typename T> class Singleton;

class EffectManager : public Singleton<EffectManager>
{
private:
	std::unique_ptr<BloodSprayEffect> mBloodSprayEffect;
	std::unique_ptr<PlayerRecoveryEffect> mPlayerRecoveryEffect;
	std::unique_ptr<RespawnPointEffect> mRespawnPointEffect;
	std::unique_ptr<LeadEffect> mLeadEffect;

private:
	// ポストエフェクト
	std::unique_ptr<Bloom> mBloom;

private:
	Player* mPlayer;

public:
	static void CreateGraphicsPipeline();

	EffectManager();
	void Init();
	void Update();
	void RenderTextureSetting();
	void DrawBloom();
	void DrawModel();

public:
	void GenerateBloodSprayEffect(const Vec3 pos);
	void GeneratePlayerRecoveryEffect(const Vec3 pos);
	void GenerateRespawnPointEffect(const Vec3 pos);
	void GenerateLeadEffect(const Vec3 pos, const Vec3 frontVec);

public:
	void SetPlayer(Player* player);

public:
	Bloom* GetBloom();

private:
	friend Singleton<EffectManager>;
};

