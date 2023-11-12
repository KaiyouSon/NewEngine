#pragma once
#include "IEffect.h"
#include "NewEngine.h"

class AttackExplosionEffect : public IEffect
{
private:
	std::unique_ptr<GPUEmitter> mEmitter;

public:
	AttackExplosionEffect();
	void Generate(const Vec3 pos);
	void Update() override;
	void Draw() override;
};