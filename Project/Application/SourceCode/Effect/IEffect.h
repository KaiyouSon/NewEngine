#pragma once
#include "Timer.h"

// エフェクトの種類
enum class EffectType
{
	None,
	LeadEffect,
	PlayerRecoveryEffect,
	LogoExplosionEffect,
};

// エフェクトのインタフェース
class IEffect
{
protected:
	bool mIsActive;
	Timer mActiveTimer;
	EffectType mEffectType;

public:
	virtual ~IEffect() {}
	virtual void Update() = 0;
	virtual void Draw() = 0;

public:
	bool GetisActive();
	EffectType GetEffectType();
};