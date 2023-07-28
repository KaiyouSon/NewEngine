#pragma once
#include "BloodSpray.h"
#include "PlayerRecoveryEffect.h"

template<typename T> class Singleton;

class EffectManager : public Singleton<EffectManager>
{
private:
	std::unique_ptr<BloodSpray> bloodSpray_;
	std::unique_ptr<PlayerRecoveryEffect> playerRecoveryEffect_;

public:
	EffectManager();
	void Init();
	void Update();
	void DrawModel();

public:
	void GenerateBloodSprayEffect(const Vec3 pos);
	void GeneratePlayerRecoveryEffect(const Vec3 pos);

private:
	friend Singleton<EffectManager>;
};

