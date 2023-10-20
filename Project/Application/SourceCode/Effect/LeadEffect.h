#pragma once
#include "NewEngine.h"

class LeadEffect
{
private:
	std::unique_ptr<GPUEmitter> mEmitter;
	Vec3 mVec;

public:
	LeadEffect();
	void Generate(const Vec3 pos, const Vec3 frontVec);
	void Update();
	void Draw();
};
