#include "RollMotion.h"
#include "HumanoidBody.h"
#include "PlayerBody.h"
#include "CollisionManager.h"

RollMotion::RollMotion()
{
	mMotion = MotionManager::GetMotion("Roll");
}

void RollMotion::Init(HumanoidBody* human)
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

	moveEase_.SetEaseTimer(35);
	moveEase_.SetPowNum(3);

	if (human->GetPart(PartID::Body)->rot.x >= Radian(360))
	{
		human->GetPart(PartID::Body)->rot.x -= Radian(360);
	}
}
void RollMotion::Update(HumanoidBody* human)
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

void RollMotion::CurrentStepInit(HumanoidBody* human)
{
	if (mStep == 0)
	{
		Step0Init(human);
	}
}
void RollMotion::CurrentStepUpdate(HumanoidBody* human)
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
	else if (mStep == 3)
	{
		Step3Update(human);
	}
}

void RollMotion::Step0Init(HumanoidBody* human)
{
	Player* player = static_cast<Player*>(human->iParent);

	moveEase_.Reset();

	// UŒ‚ƒ‚[ƒVƒ‡ƒ“‚Åi‚Þ‹——£‚ÌŒvŽZ
	player->CalcFrontVec();
	length_ = 30;// CollisionManager::GetInstance()->CalcPlayerDisToFront(human->parent->mFrontVec, 30);

	// Œ»Ý‚ÌÀ•W‚ðŽæ“¾
	startPos_ = human->pos;
	endPos_ = startPos_ + player->mFrontVec.Norm() * length_;

	// “ü—Í‚µ‚½Œã‚Ì‰ñ“]Šp‚ðŽæ“¾
	rotY_ = atan2f(player->mFrontVec.x, player->mFrontVec.z);
}
void RollMotion::Step0Update(HumanoidBody* human)
{
	Player* player = static_cast<Player*>(human->iParent);

	human->pos = moveEase_.InOut(startPos_, endPos_);
	human->rot.y = rotY_;
	player->mMoveVel = endPos_ - startPos_;

	moveEase_.Update();

	if (mEase.GetisEnd() == true)
	{
		SoundManager::Play("RollSE");
	}
}
void RollMotion::Step1Update(HumanoidBody* human)
{
	Player* player = static_cast<Player*>(human->iParent);

	human->GetPart(PartID::Body)->pos.y = mEase.In(0, -3);
	human->pos = moveEase_.InOut(startPos_, endPos_);
	player->mMoveVel = endPos_ - startPos_;

	moveEase_.Update();

	if (mEase.GetisEnd() == true)
	{
		//human->GetPart(PartID::Body)->rot.x = 30;
	}
}
void RollMotion::Step2Update(HumanoidBody* human)
{
	Player* player = static_cast<Player*>(human->iParent);

	human->GetPart(PartID::Body)->pos.y = mEase.Lerp(-3.f, -0.7f);
	human->pos = moveEase_.InOut(startPos_, endPos_);
	player->mMoveVel = endPos_ - startPos_;

	moveEase_.Update();

	if (mEase.GetisEnd() == true)
	{
		mIsCanChangeMotion = true;
	}
}
void RollMotion::Step3Update(HumanoidBody* human)
{
	Player* player = static_cast<Player*>(human->iParent);

	human->GetPart(PartID::Body)->pos.y = mEase.Out(-0.7f, 0.f);
	human->pos = moveEase_.InOut(startPos_, endPos_);
	player->mMoveVel = endPos_ - startPos_;

	moveEase_.Update();

	if (mEase.GetisEnd() == true)
	{
		human->GetPart(PartID::Body)->rot.x = 0;
	}
}
