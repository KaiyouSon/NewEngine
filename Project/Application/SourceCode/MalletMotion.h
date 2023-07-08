#pragma once
#include "IWeaponMotion.h"

// 槌関連のモーション
class MalletMotion : public IWeaponMotion
{
private:
	Vec3 startPos_;
	float startRotY_;
	float endRotY_;
	float length_;

private:// コンボ 1 ～ 4

	void CurrentStepInit(HumanoidBody* human);
	void CurrentStepUpdate(HumanoidBody* human);

	// 弱攻撃
	void WeakStep1Init(HumanoidBody* human);
	void WeakStep1Update(HumanoidBody* human);
	void WeakStep2Update(HumanoidBody* human);

	// 強攻撃
	void HeavyStep1Update(HumanoidBody* human);
	void HeavyStep2Init(HumanoidBody* human);
	void HeavyStep2Update(HumanoidBody* human);


public:
	MalletMotion();
	void Init(HumanoidBody* human) override;
	void WeakMotion(HumanoidBody* human) override;
	void HeavyMotion(HumanoidBody* human) override;
};

