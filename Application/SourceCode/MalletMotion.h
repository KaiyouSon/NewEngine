#pragma once
#include "IWeaponMotion.h"

// �Ɗ֘A�̃��[�V����
class MalletMotion : public IWeaponMotion
{
private:
	Vec3 curPos_;
	float curRotY_;
	float nextRotY_;

private:
	// �肪���Ɉ�����
	void Step0MotionInit(HumanoidBody* human);
	void Step0MotionUpdate(HumanoidBody* human);

	// �肪�O�ɓ|�����i�U������Ƃ��j
	void Step1MotionInit(HumanoidBody* human);
	void Step1MotionUpdate(HumanoidBody* human);

	// �ҋ@�ɖ߂鎞
	void Step2MotionInit(HumanoidBody* human);
	void Step2MotionUpdate(HumanoidBody* human);

	// ���݂̉�]�p���擾
	void CalcCurrentRot(HumanoidBody* human);

public:
	void Init() override;
	void AttackMotion(HumanoidBody* human) override;

};

