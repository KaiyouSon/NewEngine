#pragma once
#include "IWeaponMotion.h"

// 讒碁未騾｣縺ｮ繝｢繝ｼ繧ｷ繝ｧ繝ｳ
class MalletMotion : public IWeaponMotion
{
private:
	Vec3 startPos_;
	float startRotY_;
	float endRotY_;
	float length_;
	Easing moveEase_;

	float startPosY;
	float endPosY;


private:// 繧ｳ繝ｳ繝・1 ・・4

	void CurrentStepInit(PlayerBody* human);
	void CurrentStepUpdate(PlayerBody* human);

	// 蠑ｱ謾ｻ謦・
	void WeakStep1Init(PlayerBody* human);
	void WeakStep1Update(PlayerBody* human);

	// 蠑ｷ謾ｻ謦・
	void HeavyStep0Init(PlayerBody* human);
	void HeavyStep0Update(PlayerBody* human);
	void HeavyStep1Init();
	void HeavyStep1Update(PlayerBody* human);
	void HeavyStep2Init(PlayerBody* human);
	void HeavyStep2Update(PlayerBody* human);
	void HeavyStep3Init();
	void HeavyStep3Update(PlayerBody* human);
	void HeavyStep4Init();
	void HeavyStep4Update(PlayerBody* human);
	void HeavyStep5Init();
	void HeavyStep5Update(PlayerBody* human);
	void HeavyStep6Init();
	void HeavyStep6Update(PlayerBody* human);

	// 繝舌ャ繧ｯ謾ｻ謦・
	void BackStep0Init(PlayerBody* human);
	void BackStep0Update(PlayerBody* human);
	void BackStep2Init(PlayerBody* human);
	void BackStep2Update(PlayerBody* human);
	void BackStep3Update(PlayerBody* human);

	// 蝗櫁ｻ｢謾ｻ謦・
	void RollStep0Init(PlayerBody* human);
	void RollStep0Update(PlayerBody* human);
	void RollStep1Update(PlayerBody* human);

public:
	MalletMotion();
	void Init(HumanoidBody* human) override;
	void WeakMotion(PlayerBody* human) override;
	void HeavyMotion(PlayerBody* human) override;
	void BackMotion(PlayerBody* human) override;
	void RollMotion(PlayerBody* human) override;
};
