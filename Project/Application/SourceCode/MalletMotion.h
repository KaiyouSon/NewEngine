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

	void CurrentStepInit(PlayerBody* human);
	void CurrentStepUpdate(PlayerBody* human);

	// ãUŒ‚
	void WeakStep1Init(PlayerBody* human);
	void WeakStep1Update(PlayerBody* human);
	void WeakStep2Update(PlayerBody* human);

	// ‹­UŒ‚
	void HeavyStep1Update(PlayerBody* human);
	void HeavyStep2Init(PlayerBody* human);
	void HeavyStep2Update(PlayerBody* human);
	void HeavyStep5Update(PlayerBody* human);

	// ƒoƒbƒNUŒ‚
	void BackStep0Init(PlayerBody* human);
	void BackStep0Update(PlayerBody* human);
	void BackStep2Init(PlayerBody* human);
	void BackStep2Update(PlayerBody* human);
	void BackStep3Update(PlayerBody* human);

	// ‰ñ“]UŒ‚
	void RollStep0Init(PlayerBody* human);
	void RollStep0Update(PlayerBody* human);
	void RollStep1Update(PlayerBody* human);

public:
	MalletMotion();
	void Init(PlayerBody* human) override;
	void WeakMotion(PlayerBody* human) override;
	void HeavyMotion(PlayerBody* human) override;
	void BackMotion(PlayerBody* human) override;
	void RollMotion(PlayerBody* human) override;
};

