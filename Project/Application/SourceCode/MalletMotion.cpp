#include "MalletMotion.h"
#include "HumanoidBody.h"
#include "CollisionManager.h"

MalletMotion::MalletMotion()
{
	Init();
}

void MalletMotion::Init()
{
	isPlay_ = false;
	isInit_ = false;
	isCanChangeMotion_ = false;
	isCalcCollider_ = false;
	step_ = 0;
	ease_.Reset();
	startRots_.clear();
	comboMaxCount_ = 5;

	// �Đ��I��������̏�����
	if (comboCount_ > comboMaxCount_)
	{
		// �R���{���̏��������Ȃ�����
		ResetComboCount();
	}
}
void MalletMotion::AttackMotion(HumanoidBody* human)
{
	if (isPlay_ == false)
	{
		Init();
		isPlay_ = true;
	}

	// �肪���Ɉ�����
	if (step_ == 0)
	{
		if (isInit_ == false)
		{
			Step0MotionInit(human);
			isInit_ = true;
		}
		Step0MotionUpdate(human);
	}
	// �肪�O�ɓ|�����i�U������Ƃ��j
	else if (step_ == 1)
	{
		if (isInit_ == false)
		{
			Step1MotionInit(human);
			isInit_ = true;
		}
		Step1MotionUpdate(human);
	}
	// �ҋ@�ɖ߂鎞
	else if (step_ == 2)
	{
		if (isInit_ == false)
		{
			Step2MotionInit(human);
			isInit_ = true;
		}
		Step2MotionUpdate(human);
	}
}

// �肪���Ɉ�����
void MalletMotion::Step0MotionInit(HumanoidBody* human)
{
	// ���݂̉�]�p���擾
	CalcCurrentRot(human);

	step_ = 0;

	isCanChangeMotion_ = false;

	ComboSetting();

}
void MalletMotion::Step0MotionUpdate(HumanoidBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Head; i < startRots_.size(); i++)
	{
		// �e���ʂ��Ƃ̉�]���Ԃ���
		human->GetPart((PartID)i)->rot = ease_.Out(startRots_[i], endRots_[i]);
	}

	// ���
	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_ = 1;
		isInit_ = false;
	}
}

// �肪�O�ɓ|�����i�U������Ƃ��j
void MalletMotion::Step1MotionInit(HumanoidBody* human)
{
	// �O�x�N�g���̌v�Z
	human->CalcFrontVec();

	// �U�����[�V�����Ői�ދ����̌v�Z
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(human->frontVec, 10);

	// ���݂̍��W���擾
	startPos_ = human->pos;

	// ���͂�����̉�]�p���擾
	startRotY_ = human->rot.y;
	endRotY_ = atan2f(human->frontVec.x, human->frontVec.z);

	// ���݂̉�]�p���擾
	CalcCurrentRot(human);

	// �����蔻��L��
	isCalcCollider_ = true;

	ComboSetting();
}
void MalletMotion::Step1MotionUpdate(HumanoidBody* human)
{
	prevPos_ = human->pos;

	const Vec3 endPos = startPos_ + human->frontVec.Norm() * length_;

	human->pos = ease_.InOut(startPos_, endPos);
	human->rot.y = ease_.InOut(startRotY_, endRotY_);
	human->parent->moveVel = endPos - startPos_;

	for (uint32_t i = (uint32_t)PartID::Head; i < startRots_.size(); i++)
	{
		// �e���ʂ��Ƃ̉�]���Ԃ���
		human->GetPart((PartID)i)->rot = ease_.InOut(startRots_[i], endRots_[i]);
	}

	// ���
	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_ = 2;
		isInit_ = false;

		isCanChangeMotion_ = true;

		// �����蔻�薳��
		isCalcCollider_ = false;
	}
}

// �ҋ@�ɖ߂鎞
void MalletMotion::Step2MotionInit(HumanoidBody* human)
{
	// ���݂̉�]�p���擾
	CalcCurrentRot(human);

	ComboSetting();
}
void MalletMotion::Step2MotionUpdate(HumanoidBody* human)
{
	for (uint32_t i = 0; i < startRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.InOut(startRots_[i], endRots_[i]);
	}

	// ���
	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_ = 0;

		isPlay_ = false;
		isInit_ = false;
		isCanChangeMotion_ = false;

		Init();
	}
}

// �e�R���{�̐ݒ�
void MalletMotion::ComboSetting()
{
	if (comboCount_ < comboMaxCount_)
	{
		if (step_ == 0)
		{
			// step0 ���̃C�[�W���O�̃p�����[�^�[
			if (comboCount_ == 0)
			{
				ease_.SetEaseTimer(15);
			}
			else
			{
				ease_.SetEaseTimer(10);
			}
			ease_.SetPowNum(2);
			ease_.Reset();

			endRots_[(uint32_t)PartID::Head] = Radian(Vec3(-5, -55, 0));		// ��
			endRots_[(uint32_t)PartID::Body] = Radian(Vec3(10, 55, 10));		// ��
			endRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(Random::RangeF(-60, -40), 45, 70));	// �E�r
			endRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(Random::RangeF(-40, 0), 65, -90));	// �E��
			endRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(-20, 15, -20));	// ���r
			endRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-20, 15, 0));	// ����
			endRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(-15, 0, 20));	// �E������
			endRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(30, 15, 0));		// �E��
			endRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-70, -30, -15));// �E������
			endRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(50, 0, 0));		// ����
		}
		else if (step_ == 1)
		{
			// step1 ���̃C�[�W���O�̃p�����[�^�[
			if (comboCount_ == 0)
			{
				ease_.SetEaseTimer(25);
			}
			else
			{
				ease_.SetEaseTimer(20);
			}
			ease_.SetPowNum(5);
			ease_.Reset();

			// ��]�p�x
			endRots_[(uint32_t)PartID::Head] = Radian(Vec3(-10, 55, 0));		// ��
			endRots_[(uint32_t)PartID::Body] = Radian(Vec3(20, -55, -20));		// ��
			endRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(-20, 40, 0));	// �E�r
			endRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-40, 0, 0));	// �E��
			endRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(30, 0, -20));		// ���r
			endRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-60, 0, 0));		// ����
			endRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(0, -45, 70));	// �E������
			endRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(0, 0, -40));		// �E��
			endRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-5, 0, 0));		// �E������
			endRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(15, 0, 0));		// ����
		}
		else if (step_ == 2)
		{
			// step2 ���̃C�[�W���O�̃p�����[�^�[
			ease_.SetEaseTimer(40);
			ease_.SetPowNum(2);
			ease_.Reset();

			for (uint32_t i = 0; i < startRots_.size(); i++)
			{
				endRots_[i] = 0;
			}
		}
	}
	else if (comboCount_ == comboMaxCount_)
	{
		if (step_ == 0)
		{
			// step0 ���̃C�[�W���O�̃p�����[�^�[
			ease_.SetEaseTimer(20);
			ease_.SetPowNum(2);
			ease_.Reset();

			endRots_[(uint32_t)PartID::Head] = Radian(Vec3(-5, -65, 0));		// ��
			endRots_[(uint32_t)PartID::Body] = Radian(Vec3(10, 65, 10));		// ��
			endRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(-60, 45, 70));	// �E�r
			endRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-30, 90, -90));	// �E��
			endRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(-20, 15, -20));	// ���r
			endRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-20, 15, 0));	// ����
			endRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(-15, 0, 20));	// �E������
			endRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(30, 15, 0));		// �E��
			endRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-70, -30, -15));// �E������
			endRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(50, 0, 0));		// ����
		}
		else if (step_ == 1)
		{
			// step1 ���̃C�[�W���O�̃p�����[�^�[
			ease_.SetEaseTimer(30);
			ease_.SetPowNum(5);
			ease_.Reset();

			endRots_[(uint32_t)PartID::Head] = Radian(Vec3(-20, 55, 0));		// ��
			endRots_[(uint32_t)PartID::Body] = Radian(Vec3(35, -55, -35));		// ��
			endRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(-30, 40, 0));	// �E�r
			endRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-15, 0, 0));	// �E��
			endRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(50, 0, -20));		// ���r
			endRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-60, 0, 0));		// ����
			endRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(0, -45, 100));	// �E������
			endRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(0, 0, -60));		// �E��
			endRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-5, 0, 0));		// �E������
			endRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(15, 0, 0));		// ����
		}
		else if (step_ == 2)
		{
			// step2 ���̃C�[�W���O�̃p�����[�^�[
			ease_.SetEaseTimer(60);
			ease_.SetPowNum(2);
			ease_.Reset();

			for (uint32_t i = 0; i < startRots_.size(); i++)
			{
				endRots_[i] = 0;
			}
		}
	}
}

// ���݂̉�]�p���擾
void MalletMotion::CalcCurrentRot(HumanoidBody* human)
{
	startRots_.resize(human->GetPartsSize());
	endRots_.resize(human->GetPartsSize());
	for (uint32_t i = 0; i < human->GetPartsSize(); i++)
	{
		startRots_[i] = human->GetPart((PartID)i)->rot;
	}
}
