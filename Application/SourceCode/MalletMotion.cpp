#include "MalletMotion.h"
#include "HumanoidBody.h"

void MalletMotion::Init()
{
	isInit_ = false;
	isPlay_ = false;
	isCalcCollider_ = false;
	step_ = 0;
	ease_.Reset();
	curRots_.clear();
}
void MalletMotion::AttackMotion(HumanoidBody* human)
{
	if (isPlay_ == false)
	{
		return;
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
	// step0 ���̃C�[�W���O�̃p�����[�^�[
	ease_.SetEaseTimer(15);
	ease_.SetPowNum(2);
	ease_.Reset();

	// ���݂̉�]�p���擾
	CalcCurrentRot(human);

	step_ = 0;

}
void MalletMotion::Step0MotionUpdate(HumanoidBody* human)
{
	// ��
	Vec3 bodyMove
	{
		ease_.Out(curRots_[(uint32_t)PartID::Body].x, Radian(10)),
		ease_.Out(curRots_[(uint32_t)PartID::Body].y, Radian(55)),
		ease_.Out(curRots_[(uint32_t)PartID::Body].z, Radian(10)),
	};
	human->GetPart(PartID::Body)->rot = bodyMove;

	// ��
	Vec3 headMove
	{
		ease_.Out(curRots_[(uint32_t)PartID::Head].x, Radian(-5)),
		ease_.Out(curRots_[(uint32_t)PartID::Head].y, Radian(-55)),
		ease_.Out(curRots_[(uint32_t)PartID::Head].z, Radian(+0)),
	};
	human->GetPart(PartID::Head)->rot = headMove;

	// �E�r
	Vec3 rightArmMove =
	{
		ease_.Out(curRots_[(uint32_t)PartID::RightArm].x, Radian(-60)),
		ease_.Out(curRots_[(uint32_t)PartID::RightArm].y, Radian(+45)),
		ease_.Out(curRots_[(uint32_t)PartID::RightArm].z, Radian(+70)),
	};
	human->GetPart(PartID::RightArm)->rot = rightArmMove;

	// �E��
	Vec3 rightHandMove =
	{
		ease_.Out(curRots_[(uint32_t)PartID::RightHand].x, Radian(-20)),
		ease_.Out(curRots_[(uint32_t)PartID::RightHand].x, Radian(+65)),
		ease_.Out(curRots_[(uint32_t)PartID::RightHand].z, Radian(-90)),
	};
	human->GetPart(PartID::RightHand)->rot = rightHandMove;

	// ���r
	Vec3 leftArmMove =
	{
		ease_.Out(curRots_[(uint32_t)PartID::LeftArm].x, Radian(-20)),
		ease_.Out(curRots_[(uint32_t)PartID::LeftArm].y, Radian(+15)),
		ease_.Out(curRots_[(uint32_t)PartID::LeftArm].z, Radian(-20)),
	};
	human->GetPart(PartID::LeftArm)->rot = leftArmMove;

	// ����
	Vec3 leftHandMove =
	{
		ease_.Out(curRots_[(uint32_t)PartID::LeftHand].x, Radian(-20)),
		ease_.Out(curRots_[(uint32_t)PartID::LeftHand].y, Radian(+15)),
		ease_.Out(curRots_[(uint32_t)PartID::LeftHand].z, Radian(+0)),
	};
	human->GetPart(PartID::LeftHand)->rot = leftHandMove;

	// �E������
	Vec3 rightThighMove =
	{
		ease_.Out(curRots_[(uint32_t)PartID::RightThigh].x, Radian(-15)),
		ease_.Out(curRots_[(uint32_t)PartID::RightThigh].y, Radian(+0)),
		ease_.Out(curRots_[(uint32_t)PartID::RightThigh].z, Radian(+20)),
	};
	human->GetPart(PartID::RightThigh)->rot = rightThighMove;

	// �E��
	Vec3 rightLegMove =
	{
		ease_.Out(curRots_[(uint32_t)PartID::RightLeg].x, Radian(+30)),
		ease_.Out(curRots_[(uint32_t)PartID::RightLeg].y, Radian(+15)),
		ease_.Out(curRots_[(uint32_t)PartID::RightLeg].z, Radian(+0)),
	};
	human->GetPart(PartID::RightLeg)->rot = rightLegMove;

	// ��������
	Vec3 leftThighMove =
	{
		ease_.Out(curRots_[(uint32_t)PartID::LeftThigh].x, Radian(-70)),
		ease_.Out(curRots_[(uint32_t)PartID::LeftThigh].y, Radian(-30)),
		ease_.Out(curRots_[(uint32_t)PartID::LeftThigh].z, Radian(-15)),
	};
	human->GetPart(PartID::LeftThigh)->rot = leftThighMove;

	// ����
	Vec3 leftLegMove =
	{
		ease_.Out(curRots_[(uint32_t)PartID::LeftLeg].x, Radian(+50)),
		ease_.Out(curRots_[(uint32_t)PartID::LeftLeg].y, Radian(+0)),
		ease_.Out(curRots_[(uint32_t)PartID::LeftLeg].z, Radian(+0)),
	};
	human->GetPart(PartID::LeftLeg)->rot = leftLegMove;

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
	// step1 ���̃C�[�W���O�̃p�����[�^�[
	ease_.SetEaseTimer(25);
	ease_.SetPowNum(5);
	ease_.Reset();

	// �O�x�N�g���̌v�Z
	human->CalcFrontVec();

	// ���݂̍��W���擾
	curPos_ = human->pos;

	// ���͂�����̉�]�p���擾
	curRotY_ = atan2f(human->frontVec.x, human->frontVec.z);

	// ���݂̉�]�p���擾
	CalcCurrentRot(human);

	// �����蔻��L��
	isCalcCollider_ = true;
}
void MalletMotion::Step1MotionUpdate(HumanoidBody* human)
{
	const float length = 10.f;
	const Vec3 nextPos = curPos_ + human->frontVec.Norm() * length;

	human->pos = ease_.InOut(curPos_, nextPos);
	human->rot.y = curRotY_;

	// ��
	Vec3 bodyMove
	{
		ease_.InOut(curRots_[(uint32_t)PartID::Body].x, Radian(+20)),
		ease_.InOut(curRots_[(uint32_t)PartID::Body].y, Radian(-55)),
		ease_.InOut(curRots_[(uint32_t)PartID::Body].z, Radian(-20)),
	};
	human->GetPart(PartID::Body)->rot = bodyMove;

	// ��
	Vec3 headMove
	{
		ease_.InOut(curRots_[(uint32_t)PartID::Head].x, Radian(-10)),
		ease_.InOut(curRots_[(uint32_t)PartID::Head].y, Radian(+55)),
		ease_.InOut(curRots_[(uint32_t)PartID::Head].z, Radian(+0)),
	};
	human->GetPart(PartID::Head)->rot = headMove;

	// �E�r
	Vec3 rightArmMove
	{
		ease_.InOut(curRots_[(uint32_t)PartID::RightArm].x, Radian(-20)),
		ease_.InOut(curRots_[(uint32_t)PartID::RightArm].y, Radian(+40)),
		ease_.InOut(curRots_[(uint32_t)PartID::RightArm].z, Radian(+0)),
	};
	human->GetPart(PartID::RightArm)->rot = rightArmMove;

	// �E��
	Vec3 rightHandMove
	{
		ease_.InOut(curRots_[(uint32_t)PartID::RightHand].x, Radian(-40)),
		ease_.InOut(curRots_[(uint32_t)PartID::RightHand].y, Radian(+0)),
		ease_.InOut(curRots_[(uint32_t)PartID::RightHand].z, Radian(+0)),
	};
	human->GetPart(PartID::RightHand)->rot = rightHandMove;

	// ���r
	Vec3 leftArmMove
	{
		ease_.InOut(curRots_[(uint32_t)PartID::LeftArm].x, Radian(+30)),
		ease_.InOut(curRots_[(uint32_t)PartID::LeftArm].y, Radian(+0)),
		ease_.InOut(curRots_[(uint32_t)PartID::LeftArm].z, Radian(-20)),
	};
	human->GetPart(PartID::LeftArm)->rot = leftArmMove;

	// ����
	Vec3 leftHandMove
	{
		ease_.InOut(curRots_[(uint32_t)PartID::LeftHand].x, Radian(-60)),
		ease_.InOut(curRots_[(uint32_t)PartID::LeftHand].y, Radian(+0)),
		ease_.InOut(curRots_[(uint32_t)PartID::LeftHand].z, Radian(+0)),
	};
	human->GetPart(PartID::LeftHand)->rot = leftHandMove;

	// �E������
	Vec3 rightThighMove
	{
		ease_.InOut(curRots_[(uint32_t)PartID::RightThigh].x, Radian(-0)),
		ease_.InOut(curRots_[(uint32_t)PartID::RightThigh].y, Radian(-45)),
		ease_.InOut(curRots_[(uint32_t)PartID::RightThigh].z, Radian(+70)),
	};
	human->GetPart(PartID::RightThigh)->rot = rightThighMove;

	// �E��
	Vec3 rightLegMove
	{
		ease_.InOut(curRots_[(uint32_t)PartID::RightLeg].x, Radian(+0)),
		ease_.InOut(curRots_[(uint32_t)PartID::RightLeg].y, Radian(+0)),
		ease_.InOut(curRots_[(uint32_t)PartID::RightLeg].z, Radian(-40)),
	};
	human->GetPart(PartID::RightLeg)->rot = rightLegMove;

	// ��������
	Vec3 leftThighMove
	{
		ease_.InOut(curRots_[(uint32_t)PartID::LeftThigh].x, Radian(-5)),
		ease_.InOut(curRots_[(uint32_t)PartID::LeftThigh].y, Radian(+0)),
		ease_.InOut(curRots_[(uint32_t)PartID::LeftThigh].z, Radian(+0)),
	};
	human->GetPart(PartID::LeftThigh)->rot = leftThighMove;

	// ����
	Vec3 leftLegMove
	{
		ease_.InOut(curRots_[(uint32_t)PartID::LeftLeg].x, Radian(+15)),
		ease_.InOut(curRots_[(uint32_t)PartID::LeftLeg].y, Radian(+0)),
		ease_.InOut(curRots_[(uint32_t)PartID::LeftLeg].z, Radian(+0)),
	};
	human->GetPart(PartID::LeftLeg)->rot = leftLegMove;

	// ���
	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_ = 2;
		isInit_ = false;

		// �����蔻�薳��
		isCalcCollider_ = false;
	}
}

// �ҋ@�ɖ߂鎞
void MalletMotion::Step2MotionInit(HumanoidBody* human)
{
	// step1 ���̃C�[�W���O�̃p�����[�^�[
	ease_.SetEaseTimer(40);
	ease_.SetPowNum(2);
	ease_.Reset();

	// ���݂̉�]�p���擾
	CalcCurrentRot(human);
}
void MalletMotion::Step2MotionUpdate(HumanoidBody* human)
{
	for (uint32_t i = 0; i < curRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.InOut(curRots_[i], 0);
	}

	// ���
	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_ = 0;

		isPlay_ = false;
		isInit_ = false;
	}
}

void MalletMotion::CalcCurrentRot(HumanoidBody* human)
{
	curRots_.resize(human->GetPartsSize());
	for (uint32_t i = 0; i < human->GetPartsSize(); i++)
	{
		curRots_[i] = human->GetPart((PartID)i)->rot;
	}
}
