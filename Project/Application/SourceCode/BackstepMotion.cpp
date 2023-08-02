#include "BackstepMotion.h"
#include "HumanoidBody.h"
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

	moveEase_.SetEaseTimer(15);
	moveEase_.SetPowNum(2);
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
	BasePostUpdate(human);
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

	moveEase_.Reset();

	mStep = 0;

	// 攻撃モーションで進む距離の計算
	player->CalcFrontVec();
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(-player->mFrontVec, 15);

	// 現在の座標を取得
	startPos_ = human->pos;
	endPos_ = startPos_ - player->mFrontVec.Norm() * length_;

	up_ = 1;
	down_ = -0.5;
}
void BackstepMotion::Step0Update(HumanoidBody* human)
{
	Player* player = static_cast<Player*>(human->iParent);

	human->GetPart(PartID::Body)->pos.y = mEase.Interpolation(0.f, up_);
	human->pos = moveEase_.InOut(startPos_, endPos_);
	player->mMoveVel = endPos_ - startPos_;

	moveEase_.Update();
}
void BackstepMotion::Step1Update(HumanoidBody* human)
{
	Player* player = static_cast<Player*>(human->iParent);

	human->GetPart(PartID::Body)->pos.y = mEase.Interpolation(up_, down_);
	human->pos = moveEase_.InOut(startPos_, endPos_);
	player->mMoveVel = endPos_ - startPos_;

	moveEase_.Update();

	if (mEase.GetisEnd() == true)
	{
		mIsCanChangeMotion = true;
		SoundManager::Play("BackstepSE");
	}
}
void BackstepMotion::Step2Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = mEase.Interpolation(down_, 0.f);

	if (mEase.GetisEnd() == true)
	{
		mIsCanChangeMotion = false;
	}
}
