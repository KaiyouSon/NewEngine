#pragma once
#include "NewEngine.h"

class PlayerRecoveryEffect
{
private:
	bool mIsGenerate;
	Timer mTimer;

	std::unique_ptr<Emitter> mCircleEmitter;
	std::unique_ptr<Emitter> mLineEmitter;
	std::vector<ParticleParameter::PParam1> mCirclePParam;
	std::vector<ParticleParameter::PParam1> mLinePParam;

	Vec3 mStartPos;

private:
	void GenerateUpdate();

public:
	PlayerRecoveryEffect();
	void Init();
	void Generate(const Vec3 pos);
	void Update();
	void DrawModel();
};
