#include "MoveMotion.h"
#include "HumanoidBody.h"
#include "Player.h"

MoveMotion::MoveMotion()
{
}

void MoveMotion::Init(PlayerBody* human)
{
	mEase.SetEaseTimer(20);
	mEase.SetPowNum(2);
	mEase.Reset();

	mStep = 0;

	mIsInit = false;
	mIsPlay = false;

	mStartRots.resize(human->GetPartsSize());
	mEndRots.resize(human->GetPartsSize());

	//RotsInit(human);
}

// �W���M���O���[�V����
void MoveMotion::JoggingMotion(PlayerBody* human)
{
	// �ŏ��̈�񂵂��ʂ�Ȃ�������
	if (mIsPlay == false)
	{
		JoggingInit(human);
		mIsPlay = true;
	}

	if (mStep == 0)
	{
		if (mIsInit == false)
		{
			Step0Init();
			mIsInit = true;
		}
		Step0Update(human);
	}
	else if (mStep == 1)
	{
		if (mIsInit == false)
		{
			Step1Init(human);
			mIsInit = true;
		}
		Step1Update(human);
	}
	else if (mStep == 2)
	{
		if (mIsInit == false)
		{
			Step2Init(human);
			mIsInit = true;
		}
		Step2Update(human);
	}
	else if (mStep == 3)
	{
		if (mIsInit == false)
		{
			Step3Init(human);
			mIsInit = true;
		}
		Step3Update(human);
	}
}

// ���胂�[�V����
void MoveMotion::RunMotion(PlayerBody* human)
{
	// �ŏ��̈�񂵂��ʂ�Ȃ�������
	if (mIsPlay == false)
	{
		// �e���ʂ̊p�x�̏�����
		RunInit(human);
		mIsPlay = true;
	}

	if (mStep == 0)
	{
		if (mIsInit == false)
		{
			Step0Init();
			mIsInit = true;
		}
		Step0Update(human);
	}
	else if (mStep == 1)
	{
		if (mIsInit == false)
		{
			Step1Init(human);
			mIsInit = true;
		}
		Step1Update(human);
	}
	else if (mStep == 2)
	{
		if (mIsInit == false)
		{
			Step2Init(human);
			mIsInit = true;
		}
		Step2Update(human);
	}
	else if (mStep == 3)
	{
		if (mIsInit == false)
		{
			Step3Init(human);
			mIsInit = true;
		}
		Step3Update(human);
	}
}

// ���݂̎p�����瑖�胂�[�V�����ɕ��
void MoveMotion::Step0Init()
{
	mEase.SetEaseTimer(10);
	mEase.SetPowNum(2);
	mEase.Reset();
}
void MoveMotion::Step0Update(PlayerBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Body; i < mStartRots.size(); i++)
	{
		human->GetPart((PartID)i)->rot = mEase.InOut(mCurRots[i], mStartRots[i]);
	}

	human->parent->mMoveSpeed = mEase.Lerp(0, mEndSpeed);

	mEase.Update();

	if (mEase.GetisEnd() == true)
	{
		mStep = 1;
		mIsInit = false;
		mEase.Reset();
	}

	if (Pad::GetStick(PadCode::LeftStick, 300) == 0)
	{
		mStep = 2;
		mIsInit = false;
		mEase.Reset();
	}
}

// ���胂�[�V����
void MoveMotion::Step1Init(PlayerBody* human)
{
	human = nullptr;

	mEase.SetEaseTimer(20);
	mEase.SetPowNum(2);
}
void MoveMotion::Step1Update(PlayerBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Body; i < mStartRots.size(); i++)
	{
		human->GetPart((PartID)i)->rot = mEase.InOut(mStartRots[i], mEndRots[i]);
	}

	mEase.Update();

	if (mEase.GetisEnd() == true)
	{
		mEase.Reset();

		ReverceRots();
		mCount = (mCount == 0) ? 1 : 0;
	}

	if (Pad::GetStick(PadCode::LeftStick, 300) == 0)
	{
		mStep = 2;
		mIsInit = false;
		mEase.Reset();
	}
}

// �ҋ@�p���ɖ߂�
void MoveMotion::Step2Init(PlayerBody* human)
{
	mEase.SetEaseTimer(10);
	mEase.SetPowNum(2);

	mCurSpeed = human->parent->mMoveSpeed;

	CalcCurrentRot(human);
}
void MoveMotion::Step2Update(PlayerBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Body; i < mStartRots.size(); i++)
	{
		human->GetPart((PartID)i)->rot = mEase.InOut(mCurRots[i], 0);
	}
	human->parent->mMoveSpeed = mEase.Lerp(mCurSpeed, 0);
	mEase.Update();

	if (mEase.GetisEnd() == true)
	{
		mIsInit = false;
		mIsPlay = false;
		mEase.Reset();
	}
}

// ���[�u���[�V�������ς�������ɕ�Ԃ���
void MoveMotion::Step3Init(PlayerBody* human)
{
	mEase.SetEaseTimer(10);
	mEase.SetPowNum(2);
	mEase.Reset();

	mCurSpeed = human->parent->mMoveSpeed;

	if (mCount == 1)
	{
		ReverceRots();
	}
}
void MoveMotion::Step3Update(PlayerBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Body; i < mEndRots.size(); i++)
	{
		human->GetPart((PartID)i)->rot = mEase.Lerp(mCurRots[i], mEndRots[i]);
	}

	human->parent->mMoveSpeed = mEase.Lerp(mCurSpeed, mEndSpeed);

	mEase.Update();

	if (mEase.GetisEnd() == true)
	{
		mStep = 1;

		mEase.Reset();

		mIsInit = false;
		mCount = (mCount == 0) ? 1 : 0;
		ReverceRots();
	}
}

// �W���M���O�̏�����
void MoveMotion::JoggingInit(PlayerBody* human)
{
	mIsInit = false;

	mStep = mIsPlay ? 3 : 0;

	// ���݂̊p�x���v�Z
	mCurRots = human->CalcCurRots();
	// �I�����x�̎擾
	mEndSpeed = human->parent->mJoggingSpeed;

	// �e���ʂ̊p�x�̏�����
	mStartRots[(uint32_t)PartID::Head] = Radian(Vec3(0, -2, 0));
	mStartRots[(uint32_t)PartID::Body] = Radian(Vec3(5, 10, 0));
	mStartRots[(uint32_t)PartID::RightArm] = Radian(Vec3(45, 0, 5));
	mStartRots[(uint32_t)PartID::RightHand] = Radian(Vec3(-70, 0, 0));
	mStartRots[(uint32_t)PartID::LeftArm] = Radian(Vec3(-25, 0, -5));
	mStartRots[(uint32_t)PartID::LeftHand] = Radian(Vec3(-50, 0, 0));
	mStartRots[(uint32_t)PartID::RightThigh] = Radian(Vec3(-60, 0, 0));
	mStartRots[(uint32_t)PartID::RightLeg] = Radian(Vec3(30, 0, 0));
	mStartRots[(uint32_t)PartID::LeftThigh] = Radian(Vec3(40, 0, 0));
	mStartRots[(uint32_t)PartID::LeftLeg] = Radian(Vec3(30, 0, 0));

	mEndRots[(uint32_t)PartID::Head] = Radian(Vec3(0, 2, 0));
	mEndRots[(uint32_t)PartID::Body] = Radian(Vec3(5, -10, 0));
	mEndRots[(uint32_t)PartID::RightArm] = Radian(Vec3(-25, 0, 5));
	mEndRots[(uint32_t)PartID::RightHand] = Radian(Vec3(-50, 0, 0));
	mEndRots[(uint32_t)PartID::LeftArm] = Radian(Vec3(45, 0, -5));
	mEndRots[(uint32_t)PartID::LeftHand] = Radian(Vec3(-70, 0, 0));
	mEndRots[(uint32_t)PartID::RightThigh] = Radian(Vec3(40, 0, 0));
	mEndRots[(uint32_t)PartID::RightLeg] = Radian(Vec3(10, 0, 0));
	mEndRots[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-60, 0, 0));
	mEndRots[(uint32_t)PartID::LeftLeg] = Radian(Vec3(10, 0, 0));
}

// ����̏�����
void MoveMotion::RunInit(PlayerBody* human)
{
	mIsInit = false;

	mStep = mIsPlay ? 3 : 0;

	// ���݂̊p�x���v�Z
	mCurRots = human->CalcCurRots();
	// �I�����x�̎擾
	mEndSpeed = human->parent->mRunSpeed;

	// �e���ʂ̊p�x�̏�����
	mStartRots[(uint32_t)PartID::Head] = Radian(Vec3(0, -10, 0));
	mStartRots[(uint32_t)PartID::Body] = Radian(Vec3(15, 10, 0));
	mStartRots[(uint32_t)PartID::RightArm] = Radian(Vec3(60, 0, 10));
	mStartRots[(uint32_t)PartID::RightHand] = Radian(Vec3(-80, 0, 0));
	mStartRots[(uint32_t)PartID::LeftArm] = Radian(Vec3(-40, 0, -10));
	mStartRots[(uint32_t)PartID::LeftHand] = Radian(Vec3(-50, 0, 0));
	mStartRots[(uint32_t)PartID::RightThigh] = Radian(Vec3(-80, 0, 0));
	mStartRots[(uint32_t)PartID::RightLeg] = Radian(Vec3(15, 0, 0));
	mStartRots[(uint32_t)PartID::LeftThigh] = Radian(Vec3(50, 0, 0));
	mStartRots[(uint32_t)PartID::LeftLeg] = Radian(Vec3(30, 0, 0));

	mEndRots[(uint32_t)PartID::Head] = Radian(Vec3(0, 10, 0));
	mEndRots[(uint32_t)PartID::Body] = Radian(Vec3(15, -10, 0));
	mEndRots[(uint32_t)PartID::RightArm] = Radian(Vec3(-40, 0, 10));
	mEndRots[(uint32_t)PartID::RightHand] = Radian(Vec3(-50, 0, 0));
	mEndRots[(uint32_t)PartID::LeftArm] = Radian(Vec3(60, 0, -10));
	mEndRots[(uint32_t)PartID::LeftHand] = Radian(Vec3(-80, 0, 0));
	mEndRots[(uint32_t)PartID::RightThigh] = Radian(Vec3(50, 0, 0));
	mEndRots[(uint32_t)PartID::RightLeg] = Radian(Vec3(30, 0, 0));
	mEndRots[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-80, 0, 0));
	mEndRots[(uint32_t)PartID::LeftLeg] = Radian(Vec3(15, 0, 0));
}

// ���݂̊p�x���v�Z����
void MoveMotion::CalcCurrentRot(PlayerBody* human)
{
	mCurRots.resize(human->GetPartsSize());

	for (uint32_t i = 0; i < human->GetPartsSize(); i++)
	{
		mCurRots[i] = human->GetPart((PartID)i)->rot;
	}
}

// �J�n��]�p�x�ƏI����]�p�x�𔽓]����
void MoveMotion::ReverceRots()
{
	std::vector<Vec3> temp;
	temp.resize(mStartRots.size());

	for (uint32_t i = 0; i < mStartRots.size(); i++)
	{
		// �X�^�[�g���i�[����
		temp[i] = mStartRots[i];

		// �G���h���X�^�[�g�ɏ㏑������
		mStartRots[i] = mEndRots[i];

		//�i�[�����X�^�[�g���G���h�ɏ㏑������
		mEndRots[i] = temp[i];
	}
}

bool MoveMotion::GetisPlay()
{
	return mIsPlay;
}
