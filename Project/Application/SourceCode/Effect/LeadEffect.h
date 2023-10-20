#pragma once
#include "NewEngine.h"
#include "IEffect.h"

class LeadEffect : public IEffect
{
private:
	std::unique_ptr<GPUEmitter> mEmitter;
	Vec3 mVec;

public:
	LeadEffect();
	void Generate(const Vec3 pos, const Vec3 frontVec);
	void Update() override;
	void Draw() override;
};