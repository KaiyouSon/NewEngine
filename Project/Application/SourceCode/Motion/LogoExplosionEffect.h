#pragma once
#include "NewEngine.h"
#include "IEffect.h"

// タイトルロゴ爆散エフェクトのクラス
class LogoExplosionEffect : public IEffect
{
private:
	std::unique_ptr<ParticleMesh> mParticleMesh;
	bool mIsExplosion;

public:
	LogoExplosionEffect();
	void Generate(const Vec3 pos, const Vec3 rot, const Vec3 scale);
	void Update() override;
	void Draw() override;

public:
	void SetisExplosion(const bool isExplosion);
};

