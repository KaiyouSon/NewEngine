#pragma once
#include "NewEngine.h"
#include "IEffect.h"

class AirEffect : public IEffect
{
private:
	std::unique_ptr<GPUEmitter> mEmitter;
	Timer mTimer;

	std::vector<ParticleParameter::PParam1> mPParam;

	Vec3 mStartPos;

private:
	void GenerateUpdate();

public:
	AirEffect();
	void Generate(const Vec3 pos);
	void Update() override;
	void Draw() override;
};
