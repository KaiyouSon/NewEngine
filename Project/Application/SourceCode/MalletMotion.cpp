#include "MalletMotion.h"
#include "HumanoidBody.h"
#include "CollisionManager.h"

MalletMotion::MalletMotion()
{
	motions_.emplace_back(MotionManager::GetMotion("MalletWeakAttack1"));
	motions_.emplace_back(MotionManager::GetMotion("MalletWeakAttack2"));
	motions_.emplace_back(MotionManager::GetMotion("MalletWeakAttack3"));
	motions_.emplace_back(MotionManager::GetMotion("MalletHeavyAttack"));
	motions_.emplace_back(MotionManager::GetMotion("MalletBackAttack"));
	motions_.emplace_back(MotionManager::GetMotion("MalletRollAttack"));
}

void MalletMotion::Init(HumanoidBody* human)
{
	isPlay_ = false;
	isInit_ = false;
	isCanChangeMotion_ = false;
	isCalcCollider_ = false;
	step_ = 0;
	ease_.Reset();
	curRots_.clear();
	comboMaxCount_ = 5;

	curRots_.resize(human->GetPartsSize());
	endRots_.resize(human->GetPartsSize());

	// �Đ��I��������̏�����
	if (comboCount_ > comboMaxCount_)
	{
		// �R���{���̏��������Ȃ�����
		ResetComboCount();
	}
}
void MalletMotion::WeakMotion(HumanoidBody* human)
{
	if (isPlay_ == false)
	{
		isPlay_ = true;
		attackType_ = AttackType::Weak;
	}

	ease_.Update();

	if (isInit_ == false)
	{
		CurrentStepInit(human);
		if (comboCount_ == 1)
		{
			BaseInit(human, 0);
		}
		else if (comboCount_ < comboMaxCount_)
		{
			BaseInit(human, 1);
		}
		else
		{
			BaseInit(human, 2);
		}
	}
	CurrentStepUpdate(human);

	if (comboCount_ == 1)
	{
		BaseUpdate(human, 0);
	}
	else if (comboCount_ < comboMaxCount_)
	{
		BaseUpdate(human, 1);
	}
	else
	{
		BaseUpdate(human, 2);
	}
}
void MalletMotion::HeavyMotion(HumanoidBody* human)
{
	if (isPlay_ == false)
	{
		isPlay_ = true;
		attackType_ = AttackType::Heavy;
	}
	ease_.Update();

	if (isInit_ == false)
	{
		CurrentStepInit(human);
		BaseInit(human, 3);
	}
	CurrentStepUpdate(human);
	BaseUpdate(human, 3);
}
void MalletMotion::BackMotion(HumanoidBody* human)
{
	if (isPlay_ == false)
	{
		isPlay_ = true;
		attackType_ = AttackType::Back;
	}
	ease_.Update();

	if (isInit_ == false)
	{
		CurrentStepInit(human);
		BaseInit(human, 4);
	}
	CurrentStepUpdate(human);
	BaseUpdate(human, 4);
}
void MalletMotion::RollMotion(HumanoidBody* human)
{
	if (isPlay_ == false)
	{
		isPlay_ = true;
		attackType_ = AttackType::Roll;
	}
	ease_.Update();

	if (isInit_ == false)
	{
		CurrentStepInit(human);
		BaseInit(human, 5);
	}
	CurrentStepUpdate(human);
	BaseUpdate(human, 5);
}

void MalletMotion::CurrentStepInit(HumanoidBody* human)
{
	if (attackType_ == AttackType::Weak)
	{
		if (step_ == 1)
		{
			WeakStep1Init(human);
		}
	}
	else if (attackType_ == AttackType::Heavy)
	{
		if (step_ == 2)
		{
			HeavyStep2Init(human);
		}
	}
}
void MalletMotion::CurrentStepUpdate(HumanoidBody* human)
{
	if (attackType_ == AttackType::Weak)
	{
		if (step_ == 1)
		{
			WeakStep1Update(human);
		}
		else if (step_ == 2)
		{
			WeakStep2Update(human);
		}
	}
	else if (attackType_ == AttackType::Heavy)
	{
		if (step_ == 1)
		{
			HeavyStep1Update(human);
		}
		else if (step_ == 2)
		{
			HeavyStep2Update(human);
		}
		else if (step_ == 5)
		{
			HeavyStep5Update(human);
		}
	}
}

// ��U��
void MalletMotion::WeakStep1Init(HumanoidBody* human)
{
	// �O�x�N�g���̌v�Z
	human->parent->CalcFrontVec();

	// �U�����[�V�����Ői�ދ����̌v�Z
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(human->parent->frontVec_, 8);

	// ���݂̍��W���擾
	startPos_ = human->pos;

	// ���͂�����̉�]�p���擾
	startRotY_ = human->rot.y;
	endRotY_ = atan2f(human->parent->frontVec_.x, human->parent->frontVec_.z);

	// �����蔻��L��
	isCalcCollider_ = true;
}
void MalletMotion::WeakStep1Update(HumanoidBody* human)
{
	// �����O�Ɉړ����鏈��
	const Vec3 endPos = startPos_ + human->parent->frontVec_.Norm() * length_;
	human->pos = ease_.InOut(startPos_, endPos);
	human->rot.y = ease_.InOut(startRotY_, endRotY_);
	human->parent->moveVel = endPos - startPos_;

	if (ease_.GetisEnd() == true)
	{
		if (comboCount_ < comboMaxCount_)
		{
			// �R���{�ł���t���t
			isCanChangeMotion_ = true;
		}

		// �����蔻�薳��
		isCalcCollider_ = false;
	}
}
void MalletMotion::WeakStep2Update(HumanoidBody* human)
{
	if (ease_.GetisEnd() == true)
	{
		step_ = 0;
		isInit_ = false;
		isPlay_ = false;
		isCanChangeMotion_ = false;
	}
}

// ���U��
void MalletMotion::HeavyStep1Update(HumanoidBody* human)
{
	if (!Pad::GetTrigger(PadCode::RightTrigger, 300))
	{
		ease_.SetisEnd(true);
	}
}
void MalletMotion::HeavyStep2Init(HumanoidBody* human)
{
	// �O�x�N�g���̌v�Z
	human->parent->CalcFrontVec();

	// �U�����[�V�����Ői�ދ����̌v�Z
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(human->parent->frontVec_, 10);

	// ���݂̍��W���擾
	startPos_ = human->pos;

	// ���͂�����̉�]�p���擾
	startRotY_ = human->rot.y;
	endRotY_ = atan2f(human->parent->frontVec_.x, human->parent->frontVec_.z);

	// �����蔻��L��
	isCalcCollider_ = true;
}
void MalletMotion::HeavyStep2Update(HumanoidBody* human)
{
	// �����O�Ɉړ����鏈��
	const Vec3 endPos = startPos_ + human->parent->frontVec_.Norm() * length_;
	human->pos = ease_.InOut(startPos_, endPos);
	human->rot.y = ease_.InOut(startRotY_, endRotY_);
	human->parent->moveVel = endPos - startPos_;

	if (ease_.GetisEnd() == true)
	{
		//if (comboCount_ < comboMaxCount_)
		//{
		//	// �R���{�ł���t���t
		//	isCanChangeMotion_ = true;
		//}
	}
}
void MalletMotion::HeavyStep5Update(HumanoidBody* human)
{
	if (ease_.GetisEnd() == true)
	{
		isCanChangeMotion_ = true;
	}
}
