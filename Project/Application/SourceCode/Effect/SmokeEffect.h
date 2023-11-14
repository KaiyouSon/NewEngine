#pragma once
#include "NewEngine.h"
#include "IEffect.h"

// 煙のエフェクト
class SmokeEffect : public IEffect
{
private:
	std::unique_ptr<GPUEmitter> mEmitter;
	Timer mTimer;
	ConstantBufferData::CSmokeEffect mData;

public:
	SmokeEffect();
	void Generate();
	void Update() override;
	void Draw() override;

public:
	void Execute(const ConstantBufferData::CSmokeEffect data);
};

