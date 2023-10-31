#pragma once
#include "NewEngine.h"
#include "IEffect.h"

// ボスの攻撃の軌道エフェクト
class BossAttackTrajectoryEffect : public IEffect
{
private:
	std::unique_ptr<GPUEmitter> mEmitter;
	Vec3 mStartPos;
	Vec3 mEndPos;

public:
	BossAttackTrajectoryEffect();
	void Generate(const Vec3 startPos, const Vec3 endPos);
	void Update() override;
	void Draw() override;
};