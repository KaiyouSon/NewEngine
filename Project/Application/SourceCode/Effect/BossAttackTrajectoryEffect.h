#pragma once
#include "NewEngine.h"
#include "IEffect.h"

// ボスの攻撃の軌道エフェクト
class BossAttackTrajectoryEffect : public IEffect
{
private:
	std::unique_ptr<GPUEmitter> mEmitter;
	bool mIsGenerate;
	Vec3 mStartPos;
	Vec3 mEndPos;

public:
	BossAttackTrajectoryEffect();
	void Generate();
	void Update() override;
	void Draw() override;

public:
	void Execute(const bool isGenerate, const Vec3 startPos, const Vec3 endPos);
};