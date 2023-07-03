#include "MoveMotion.h"
#include "HumanoidBody.h"

MoveMotion::MoveMotion()
{
}

void MoveMotion::Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(20);
	ease_.SetPowNum(2);
	ease_.Reset();

	step_ = 0;

	isInit_ = false;

	RotsInit(human);
}

void MoveMotion::JoggingMotion(HumanoidBody* human)
{
	if (step_ == 0)
	{
		if (isInit_ == false)
		{
			Step0Init(human);
			isInit_ = true;
		}
		Step0Update(human);
	}
	else if (step_ == 1)
	{
		if (isInit_ == false)
		{
			Step1Init(human);
			isInit_ = true;
		}
		Step1Update(human);
	}
	else if (step_ == 2)
	{
		if (isInit_ == false)
		{
			Step2Init(human);
			isInit_ = true;
		}
		Step2Update(human);
	}
}

// ���݂̎p�����瑖�胂�[�V�����ɕ��
void MoveMotion::Step0Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(10);
	ease_.SetPowNum(2);
	ease_.Reset();
	isEnd_ = false;

	CalcCurrentRot(human);
}
void MoveMotion::Step0Update(HumanoidBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Head; i < startRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.InOut(curRots_[i], startRots_[i]);
	}
	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_ = 1;

		ease_.Reset();

		isInit_ = false;
	}
}

// ���胂�[�V����
void MoveMotion::Step1Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(20);
	ease_.SetPowNum(2);
}
void MoveMotion::Step1Update(HumanoidBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Head; i < startRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.InOut(startRots_[i], endRots_[i]);
	}

	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		ReverceRots();
		ease_.Reset();
	}

	if (Pad::GetStick(PadCode::LeftStick, 300) == 0)
	{
		step_ = 2;

		ease_.Reset();

		isInit_ = false;
	}
}

// �ҋ@�p���ɖ߂�
void MoveMotion::Step2Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(10);
	ease_.SetPowNum(2);

	CalcCurrentRot(human);
}
void MoveMotion::Step2Update(HumanoidBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Head; i < startRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.InOut(curRots_[i], 0);
	}
	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		Init(human);
		isEnd_ = true;
		ease_.Reset();
	}
}

// �p�x�̏�����
void MoveMotion::RotsInit(HumanoidBody* human)
{
	startRots_.resize(human->GetPartsSize());
	endRots_.resize(human->GetPartsSize());

	startRots_[(uint32_t)PartID::Head] = Radian(Vec3(0, -2, 0));
	startRots_[(uint32_t)PartID::Body] = Radian(Vec3(5, 10, 0));
	startRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(35, 0, 5));
	startRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-70, 0, 0));
	startRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(-15, 0, -5));
	startRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-50, 0, 0));
	startRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(-50, 0, 0));
	startRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(30, 0, 0));
	startRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(30, 0, 0));
	startRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(30, 0, 0));

	endRots_[(uint32_t)PartID::Head] = Radian(Vec3(0, 2, 0));
	endRots_[(uint32_t)PartID::Body] = Radian(Vec3(5, -10, 0));
	endRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(-15, 0, 5));
	endRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-50, 0, 0));
	endRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(35, 0, -5));
	endRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-70, 0, 0));
	endRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(30, 0, 0));
	endRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(10, 0, 0));
	endRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-50, 0, 0));
	endRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(10, 0, 0));
}

// ���݂̊p�x���v�Z����
void MoveMotion::CalcCurrentRot(HumanoidBody* human)
{
	curRots_.resize(human->GetPartsSize());

	for (uint32_t i = 0; i < human->GetPartsSize(); i++)
	{
		curRots_[i] = human->GetPart((PartID)i)->rot;
	}
}

// �J�n��]�p�x�ƏI����]�p�x�𔽓]����
void MoveMotion::ReverceRots()
{
	std::vector<Vec3> temp;
	temp.resize(startRots_.size());

	for (uint32_t i = 0; i < startRots_.size(); i++)
	{
		// �X�^�[�g���i�[����
		temp[i] = startRots_[i];

		// �G���h���X�^�[�g�ɏ㏑������
		startRots_[i] = endRots_[i];

		//�i�[�����X�^�[�g���G���h�ɏ㏑������
		endRots_[i] = temp[i];
	}
}

bool MoveMotion::GetisEnd()
{
	return isEnd_;
}
