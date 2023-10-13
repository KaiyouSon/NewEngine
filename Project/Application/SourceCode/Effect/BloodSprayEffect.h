#pragma once
#include "NewEngine.h"

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