#pragma once
#include "NewEngine.h"

// 血のエフェクト（後GPUに移す）
class BloodSprayEffect
{
private:
	std::unique_ptr<Emitter> mEmitter;
	std::vector<ParticleParameter::PParam1> mPParam;

public:
	BloodSprayEffect();
	void Init();
	void Generate(const Vec3 pos);
	void Update();
	void DrawModel();
};
