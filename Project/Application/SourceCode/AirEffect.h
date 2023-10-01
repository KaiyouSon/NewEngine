#pragma once
#include "NewEngine.h"

class AirEffect
{
private:
	Timer mTimer;

	std::unique_ptr<Emitter> mEmitter;
	std::vector<ParticleParameter::PParam1> mPParam;

	Vec3 mStartPos;

	uint32_t maxParticle;

private:
	void GenerateUpdate();

public:
	AirEffect();
	void Init();
	void Generate(const Vec3 pos);
	void Update();
	void DrawModel();
};

