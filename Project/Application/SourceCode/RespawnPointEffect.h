#pragma once
#include "NewEngine.h"

class RespawnPointEffect
{
private:
	Timer mTimer;

	std::unique_ptr<Emitter> mCircleEmitter;
	std::vector<ParticleParameter::PParam1> mCirclePParam;

	Vec3 mStartPos;

private:
	void GenerateUpdate();

public:
	RespawnPointEffect();
	void Generate(const Vec3 pos);
	void Update();
	void DrawModel();
};
