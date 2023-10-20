#pragma once
#include "NewEngine.h"
#include "IEffect.h"

class PlayerRecoveryEffect : public IEffect
{
private:
	std::unique_ptr<GPUEmitter> mCircleEmitter;
	std::unique_ptr<GPUEmitter> mLineEmitter;

	ConstantBufferData::CPlayerRecoveryEffect mCircleEmitterParam;
	ConstantBufferData::CPlayerRecoveryEffect mLineEmitterParam;

public:
	PlayerRecoveryEffect();
	void Generate(const Vec3 pos);
	void Update() override;
	void Draw() override;
};
