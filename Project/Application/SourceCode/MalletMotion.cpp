#include "MalletMotion.h"
#include "PlayerBody.h"
#include "CollisionManager.h"

MalletMotion::MalletMotion()
{
	//motions_.emplace_back(MotionManager::GetMotion("BossAttack3"));
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
	curWeaponRots_.resize(human->GetWeaponPartsSize());
	endWeaponRots_.resize(human->GetWeaponPartsSize());

	// �Đ��I��������̏�����
	if (comboCount_ > comboMaxCount_)
	{
		// �R���{���̏��������Ȃ�����
		ResetComboCount();
	}

	human->GetPart(PartID::Body)->pos.y = 0.f;

}
void MalletMotion::WeakMotion(PlayerBody* human)
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
		isInit_ = true;
	}

	if (comboCount_ == 1)
	{
		BasePrevUpdate(human, 0);
	}
	else if (comboCount_ < comboMaxCount_)
	{
		BasePrevUpdate(human, 1);
	}
	else
	{
		BasePrevUpdate(human, 2);
	}

	CurrentStepUpdate(human);

	if (comboCount_ == 1)
	{
		BasePostUpdate(human, 0);
	}
	else if (comboCount_ < comboMaxCount_)
	{
		BasePostUpdate(human, 1);
	}
	else
	{
		BasePostUpdate(human, 2);
	}
}
void MalletMotion::HeavyMotion(PlayerBody* human)
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
		isInit_ = true;
	}
	BasePrevUpdate(human, 3);
	CurrentStepUpdate(human);
	BasePostUpdate(human, 3);
}
void MalletMotion::BackMotion(PlayerBody* human)
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
		isInit_ = true;
	}
	BasePrevUpdate(human, 4);
	CurrentStepUpdate(human);
	BasePostUpdate(human, 4);
}
void MalletMotion::RollMotion(PlayerBody* human)
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
	BasePrevUpdate(human, 5);
	CurrentStepUpdate(human);
	BasePostUpdate(human, 5);
}

void MalletMotion::CurrentStepInit(PlayerBody* human)
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
		if (step_ == 0)
		{
			HeavyStep0Init(human);
		}
		else if (step_ == 1)
		{
			HeavyStep1Init(human);
		}
		else if (step_ == 2)
		{
			HeavyStep2Init(human);
		}
		else if (step_ == 3)
		{
			HeavyStep3Init(human);
		}
		else if (step_ == 4)
		{
			HeavyStep4Init(human);
		}
		else if (step_ == 5)
		{
			HeavyStep5Init(human);
		}
		else if (step_ == 6)
		{
			HeavyStep6Init(human);
		}
	}
	else if (attackType_ == AttackType::Back)
	{
		if (step_ == 0)
		{
			BackStep0Init(human);
		}
		else if (step_ == 2)
		{
			BackStep2Init(human);
		}
	}
	else if (attackType_ == AttackType::Roll)
	{
		if (step_ == 0)
		{
			RollStep0Init(human);
		}
	}
}
void MalletMotion::CurrentStepUpdate(PlayerBody* human)
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
		if (step_ == 0)
		{
			HeavyStep0Update(human);
		}
		else if (step_ == 1)
		{
			HeavyStep1Update(human);
		}
		else if (step_ == 2)
		{
			HeavyStep2Update(human);
		}
		else if (step_ == 3)
		{
			HeavyStep3Update(human);
		}
		else if (step_ == 3)
		{
			HeavyStep4Update(human);
		}
		else if (step_ == 5)
		{
			HeavyStep5Update(human);
		}
		else if (step_ == 6)
		{
			HeavyStep6Update(human);
		}
	}
	else if (attackType_ == AttackType::Back)
	{
		if (step_ == 0)
		{
			BackStep0Update(human);
		}
		else if (step_ == 2)
		{
			BackStep2Update(human);
		}
		else if (step_ == 3)
		{
			BackStep3Update(human);
		}
	}
	else if (attackType_ == AttackType::Roll)
	{
		if (step_ == 0)
		{
			RollStep0Update(human);
		}
		else if (step_ == 1)
		{
			RollStep1Update(human);
		}
	}
}

// ��U��
void MalletMotion::WeakStep1Init(PlayerBody* human)
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
void MalletMotion::WeakStep1Update(PlayerBody* human)
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
void MalletMotion::WeakStep2Update(PlayerBody* human)
{
	//if (ease_.GetisEnd() == true)
	//{
	//	step_ = 0;
	//	isInit_ = false;
	//	isPlay_ = false;
	//	isCanChangeMotion_ = false;
	//}
}

// ���U��
void MalletMotion::HeavyStep0Init(PlayerBody* human)
{
	startPosY = human->GetPart(PartID::Body)->pos.y;
	endPosY = -0.62f;
}
void MalletMotion::HeavyStep0Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.In(startPosY, endPosY);
}
void MalletMotion::HeavyStep1Init(PlayerBody* human)
{
	startPosY = endPosY;
	endPosY = -1.04f;
}
void MalletMotion::HeavyStep1Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.In(startPosY, endPosY);

	if (!Pad::GetTrigger(PadCode::RightTrigger, 300))
	{
		ease_.SetisEnd(true);
	}
}
void MalletMotion::HeavyStep2Init(PlayerBody* human)
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

	startPosY = endPosY;
	endPosY = -1.3f;

	// �����蔻��L��
	isCalcCollider_ = true;
}
void MalletMotion::HeavyStep2Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.In(startPosY, endPosY);

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
void MalletMotion::HeavyStep3Init(PlayerBody* human)
{
	startPosY = endPosY;
	endPosY = -1.85f;
}
void MalletMotion::HeavyStep3Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.In(startPosY, endPosY);

	if (ease_.GetisEnd() == true)
	{
		// �����蔻�薳��
		isCalcCollider_ = false;
	}
}
void MalletMotion::HeavyStep4Init(PlayerBody* human)
{
	startPosY = endPosY;
	endPosY = -1.2f;
}
void MalletMotion::HeavyStep4Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.In(startPosY, endPosY);
}
void MalletMotion::HeavyStep5Init(PlayerBody* human)
{
	startPosY = endPosY;
	endPosY = -0.32f;
}
void MalletMotion::HeavyStep5Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.In(startPosY, endPosY);

	if (ease_.GetisEnd() == true)
	{
		isCanChangeMotion_ = true;
	}
}
void MalletMotion::HeavyStep6Init(PlayerBody* human)
{
	startPosY = endPosY;
	endPosY = 0.f;
}
void MalletMotion::HeavyStep6Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.In(startPosY, endPosY);
}

// �o�b�N�U��
void MalletMotion::BackStep0Init(PlayerBody* human)
{
	// �O�x�N�g���̌v�Z
	human->parent->CalcFrontVec();

	// �U�����[�V�����Ői�ދ����̌v�Z
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(-human->parent->frontVec_, 2);

	// ���݂̍��W���擾
	startPos_ = human->pos;

	// ���͂�����̉�]�p���擾
	startRotY_ = human->rot.y;
	endRotY_ = atan2f(human->parent->frontVec_.x, human->parent->frontVec_.z);

	moveEase_.SetEaseTimer(5);
	moveEase_.SetPowNum(2);
	moveEase_.Reset();
}
void MalletMotion::BackStep0Update(PlayerBody* human)
{
	moveEase_.Update();

	// �����O�Ɉړ����鏈��
	const Vec3 endPos = startPos_ - human->parent->frontVec_.Norm() * length_;
	human->pos = moveEase_.InOut(startPos_, endPos);
	human->parent->moveVel = endPos - startPos_;
}
void MalletMotion::BackStep2Init(PlayerBody* human)
{
	// �O�x�N�g���̌v�Z
	human->parent->CalcFrontVec();

	// �U�����[�V�����Ői�ދ����̌v�Z
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(human->parent->frontVec_, 17.5);

	// ���݂̍��W���擾
	startPos_ = human->pos;

	// ���͂�����̉�]�p���擾
	startRotY_ = human->rot.y;
	endRotY_ = atan2f(human->parent->frontVec_.x, human->parent->frontVec_.z);

	// �����蔻��L��
	isCalcCollider_ = true;

	moveEase_.SetEaseTimer(15);
	moveEase_.SetPowNum(2);
	moveEase_.Reset();
}
void MalletMotion::BackStep2Update(PlayerBody* human)
{
	moveEase_.Update();

	// �����O�Ɉړ����鏈��
	const Vec3 endPos = startPos_ + human->parent->frontVec_.Norm() * length_;
	human->pos = moveEase_.InOut(startPos_, endPos);
	human->parent->moveVel = endPos - startPos_;

}
void MalletMotion::BackStep3Update(PlayerBody* human)
{
	moveEase_.Update();

	// �����O�Ɉړ����鏈��
	const Vec3 endPos = startPos_ + human->parent->frontVec_.Norm() * length_;
	human->pos = moveEase_.InOut(startPos_, endPos);
	human->parent->moveVel = endPos - startPos_;

	if (ease_.GetisEnd() == true)
	{
		// �����蔻�薳��
		isCalcCollider_ = false;
	}

}

// ��]�U��
void MalletMotion::RollStep0Init(PlayerBody* human)
{
	// �O�x�N�g���̌v�Z
	human->parent->CalcFrontVec();

	// �U�����[�V�����Ői�ދ����̌v�Z
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(-human->parent->frontVec_, 8);

	// ���݂̍��W���擾
	startPos_ = human->pos;

	// ���͂�����̉�]�p���擾
	startRotY_ = human->rot.y;
	endRotY_ = atan2f(human->parent->frontVec_.x, human->parent->frontVec_.z);

	// �����蔻��L��
	isCalcCollider_ = true;

	moveEase_.SetEaseTimer(30);
	moveEase_.SetPowNum(2);
	moveEase_.Reset();
}
void MalletMotion::RollStep0Update(PlayerBody* human)
{
	moveEase_.Update();

	// �����O�Ɉړ����鏈��
	const Vec3 endPos = startPos_ + human->parent->frontVec_.Norm() * length_;
	human->pos = moveEase_.InOut(startPos_, endPos);
	human->parent->moveVel = endPos - startPos_;
}
void MalletMotion::RollStep1Update(PlayerBody* human)
{
	moveEase_.Update();

	// �����O�Ɉړ����鏈��
	const Vec3 endPos = startPos_ + human->parent->frontVec_.Norm() * length_;
	human->pos = moveEase_.InOut(startPos_, endPos);
	human->parent->moveVel = endPos - startPos_;

	if (ease_.GetisEnd() == true)
	{
		// �����蔻�薳��
		isCalcCollider_ = false;
	}
}