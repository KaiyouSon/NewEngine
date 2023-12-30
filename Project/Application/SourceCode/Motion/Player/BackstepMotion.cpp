#include "BackstepMotion.h"
#include "HumanoidBody.h"
#include "Player.h"
#include "CollisionManager.h"

BackstepMotion::BackstepMotion()
{
	mMotion = MotionManager::GetMotion("Backstep");
}

void BackstepMotion::Init(HumanoidBody* human)
{
	mIsInit = false;
	mIsPlay = false;
	mIsCanChangeMotion = false;
	mStep = 0;

	mCurRots.resize(human->GetPartsSize());
	mEndRots.resize(human->GetPartsSize());
	mCurWeaponPoses.resize(human->GetWeaponPartsSize());
	mEndWeaponPoses.resize(human->GetWeaponPartsSize());
	mCurWeaponRots.resize(human->GetWeaponPartsSize());
	mEndWeaponRots.resize(human->GetWeaponPartsSize());

	mMoveEase.SetEaseTimer(15);
	mMoveEase.SetPowNum(2);
}
void BackstepMotion::Update(HumanoidBody* human)
{
	if (mIsPlay == false)
	{
		mIsPlay = true;
	}

	mEase.Update();

	if (mIsInit == false)
	{
		BaseInit(human);
		CurrentStepInit(human);
		mIsInit = true;
	}
	BasePrevUpdate(human);
	CurrentStepUpdate(human);
	BasePostUpdate();
}

void BackstepMotion::CurrentStepInit(HumanoidBody* human)
{
	if (mStep == 0)
	{
		Step0Init(human);
	}
}
void BackstepMotion::CurrentStepUpdate(HumanoidBody* human)
{
	if (mStep == 0)
	{
		Step0Update(human);
	}
	else if (mStep == 1)
	{
		Step1Update(human);
	}
	else if (mStep == 2)
	{
		Step2Update(human);
	}
}

void BackstepMotion::Step0Init(HumanoidBody* human)
{
	Player* player = static_cast<Player*>(human->iParent);

	mMoveEase.Reset();

	mStep = 0;

	player->CalcFrontVec();
	mLength = CollisionManager::GetInstance()->CalcPlayerDisToFront(-player->mFrontVec, 15);

	mStartPos = human->pos;
	mEndPos = mStartPos - player->mFrontVec.Norm() * mLength;

	mUp = 1;
	mDown = -0.5;
}
void BackstepMotion::Step0Update(HumanoidBody* human)
{
	Player* player = static_cast<Player*>(human->iParent);

	human->GetPart(PartID::Body)->pos.y = mEase.Interpolation(0.f, mUp);
	human->pos = mMoveEase.InOut(mStartPos, mEndPos);
	player->mMoveVel = mEndPos - mStartPos;

	mMoveEase.Update();
}
void BackstepMotion::Step1Update(HumanoidBody* human)
{
	Player* player = static_cast<Player*>(human->iParent);

	human->GetPart(PartID::Body)->pos.y = mEase.Interpolation(mUp, mDown);
	human->pos = mMoveEase.InOut(mStartPos, mEndPos);
	player->mMoveVel = mEndPos - mStartPos;

	mMoveEase.Update();

	if (mEase.GetisEnd() == true)
	{
		mIsCanChangeMotion = true;
		SoundManager::Play("BackstepSE");
	}
}
void BackstepMotion::Step2Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = mEase.Interpolation(mDown, 0.f);

	if (mEase.GetisEnd() == true)
	{
		mIsCanChangeMotion = false;
	}
}

