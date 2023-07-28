#pragma once
#include "NewEngine.h"

class PlayerRecoveryEffect
{
private:
	bool isGenerate;
	Timer timer_;

	std::unique_ptr<Emitter> circleEmitter_;
	std::unique_ptr<Emitter> lineEmitter_;
	std::vector<ParticleParameter::PParam1> circlePParam;
	std::vector<ParticleParameter::PParam1> linePParam;

	Vec3 startPos;

private:
	void GenerateUpdate();

public:
	PlayerRecoveryEffect();
	void Generate(const Vec3 pos);
	void Update();
	void DrawModel();
};

