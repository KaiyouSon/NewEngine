#pragma once
#include "BloodSprayEffect.h"
#include "PlayerRecoveryEffect.h"
#include "RespawnPointEffect.h"

template<typename T> class Singleton;

class EffectManager : public Singleton<EffectManager>
{
private:
	std::unique_ptr<BloodSprayEffect> mBloodSprayEffect;
	std::unique_ptr<PlayerRecoveryEffect> mPlayerRecoveryEffect;
	std::unique_ptr<RespawnPointEffect> mRespawnPointEffect;

public:
	EffectManager();
	void Init();
	void Update();
	void DrawModel();

public:
	void GenerateBloodSprayEffect(const Vec3 pos);
	void GeneratePlayerRecoveryEffect(const Vec3 pos);
	void GenerateRespawnPointEffect(const Vec3 pos);

private:
	friend Singleton<EffectManager>;
};

