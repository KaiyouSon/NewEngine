#pragma once
#include "IWeaponMotion.h"

// �Ɗ֘A�̃��[�V����
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


private:// �R���{ 1 �` 4

	void CurrentStepInit(PlayerBody* human);
	void CurrentStepUpdate(PlayerBody* human);

	// ��U��
	void WeakStep1Init(PlayerBody* human);
	void WeakStep1Update(PlayerBody* human);
	void WeakStep2Update(PlayerBody* human);

	// ���U��
	void HeavyStep0Init(PlayerBody* human);
	void HeavyStep0Update(PlayerBody* human);
	void HeavyStep1Init(PlayerBody* human);
	void HeavyStep1Update(PlayerBody* human);
	void HeavyStep2Init(PlayerBody* human);
	void HeavyStep2Update(PlayerBody* human);
	void HeavyStep3Init(PlayerBody* human);
	void HeavyStep3Update(PlayerBody* human);
	void HeavyStep4Init(PlayerBody* human);
	void HeavyStep4Update(PlayerBody* human);
	void HeavyStep5Init(PlayerBody* human);
	void HeavyStep5Update(PlayerBody* human);
	void HeavyStep6Init(PlayerBody* human);
	void HeavyStep6Update(PlayerBody* human);

	// �o�b�N�U��
	void BackStep0Init(PlayerBody* human);
	void BackStep0Update(PlayerBody* human);
	void BackStep2Init(PlayerBody* human);
	void BackStep2Update(PlayerBody* human);
	void BackStep3Update(PlayerBody* human);

	// ��]�U��
	void RollStep0Init(PlayerBody* human);
	void RollStep0Update(PlayerBody* human);
	void RollStep1Update(PlayerBody* human);

public:
	MalletMotion();
	void Init(HumanoidBody* human) override;
	void Update(HumanoidBody* human) override;
	void WeakMotion(PlayerBody* human) override;
	void HeavyMotion(PlayerBody* human) override;
	void BackMotion(PlayerBody* human) override;
	void RollMotion(PlayerBody* human) override;
};

