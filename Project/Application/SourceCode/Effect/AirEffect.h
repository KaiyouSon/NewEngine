#pragma once
#include "NewEngine.h"
#include "IEffect.h"

// 空中にあるエフェクト
class AirEffect : public IEffect
{
private:
	std::unique_ptr<GPUEmitter> mEmitter;
	Timer mTimer;

public:
	AirEffect();
	void Generate(const Vec3 pos);
	void Update() override;
	void Draw() override;
};
