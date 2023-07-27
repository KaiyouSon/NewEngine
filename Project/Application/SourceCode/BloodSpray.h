#pragma once
#include "NewEngine.h"

class BloodSpray
{
private:
	std::unique_ptr<Emitter> emitter_;
	std::vector<ParticleParameter::PParam1> pParam;

public:
	BloodSpray();
	void Generate(const Vec3 pos);
	void Update();
	void DrawModel();
};

