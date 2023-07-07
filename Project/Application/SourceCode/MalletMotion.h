#pragma once
#include "IWeaponMotion.h"

// �Ɗ֘A�̃��[�V����
class MalletMotion : public IWeaponMotion
{
private:
	Vec3 prevPos_;
	Vec3 startPos_;
	float startRotY_;
	float endRotY_;
	float length_;

private:// �R���{ 1 �` 4

	// �肪���Ɉ�����
	void Step0MotionInit(HumanoidBody* human);
	void Step0MotionUpdate(HumanoidBody* human);

	// �肪�O�ɓ|�����i�U������Ƃ��j
	void Step1MotionInit(HumanoidBody* human);
	void Step1MotionUpdate(HumanoidBody* human);

	// �ҋ@�ɖ߂鎞
	void Step2MotionInit(HumanoidBody* human);
	void Step2MotionUpdate(HumanoidBody* human);

	// �e�R���{�̐ݒ�
	void ComboSetting();

	// ���݂̉�]�p���擾
	void CalcCurrentRot(HumanoidBody* human);


public:
	MalletMotion();
	void Init(HumanoidBody* human) override;
	void AttackMotion(HumanoidBody* human) override;
	void HeavyAttackMotion(HumanoidBody* human) override;
};

