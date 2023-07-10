#pragma once
#include "IWeaponMotion.h"

// ’ÆŠÖ˜A‚Ìƒ‚[ƒVƒ‡ƒ“
class MalletMotion : public IWeaponMotion
{
private:
	Vec3 startPos_;
	float startRotY_;
	float endRotY_;
	float length_;
	Easing moveEase_;

private:// ƒRƒ“ƒ{ 1 ` 4

	void CurrentStepInit(HumanoidBody* human);
	void CurrentStepUpdate(HumanoidBody* human);

	// ãUŒ‚
	void WeakStep1Init(HumanoidBody* human);
	void WeakStep1Update(HumanoidBody* human);
	void WeakStep2Update(HumanoidBody* human);

	// ‹­UŒ‚
	void HeavyStep1Update(HumanoidBody* human);
	void HeavyStep2Init(HumanoidBody* human);
	void HeavyStep2Update(HumanoidBody* human);
	void HeavyStep5Update(HumanoidBody* human);

	// ƒoƒbƒNUŒ‚
	void BackStep0Init(HumanoidBody* human);
	void BackStep0Update(HumanoidBody* human);
	void BackStep2Init(HumanoidBody* human);
	void BackStep2Update(HumanoidBody* human);
	void BackStep3Update(HumanoidBody* human);

	// ‰ñ“]UŒ‚
	void RollStep0Init(HumanoidBody* human);
	void RollStep0Update(HumanoidBody* human);
	void RollStep1Update(HumanoidBody* human);

public:
	MalletMotion();
	void Init(HumanoidBody* human) override;
	void WeakMotion(HumanoidBody* human) override;
	void HeavyMotion(HumanoidBody* human) override;
	void BackMotion(HumanoidBody* human) override;
	void RollMotion(HumanoidBody* human) override;
};

