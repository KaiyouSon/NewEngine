#pragma once
#include "NewEngine.h"

class LeadEffect
{
private:
	Timer mTimer;

	std::unique_ptr<Emitter> mEmitter;
	std::vector<ParticleParameter::PParam1> mPParam;

	Vec3 mStartPos;
	Vec3 mFrontVec;

private:
	void GenerateUpdate();

public:
	LeadEffect();
	void Init();
	void Generate(const Vec3 pos, const Vec3 frontVec);
	void Update();
	void DrawModel();
};
