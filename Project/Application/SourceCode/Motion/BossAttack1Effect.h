#pragma once
#include "ParticleMesh.h"
#include "NewEngine.h"

class BossAttack1Effect
{
private:
	std::unique_ptr<ParticleMesh> mParticleMesh;
	Timer mLifeTimer;
	bool mIsActive;

public:
	BossAttack1Effect();
	void Generate(const Vec3 pos);
	void Update();
	void Draw();

public:
	bool GetisActive();
};

