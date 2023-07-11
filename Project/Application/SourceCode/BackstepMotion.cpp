#include "BackstepMotion.h"
#include "HumanoidBody.h"
#include "PlayerBody.h"
#include "Player.h"
#include "CollisionManager.h"

BackstepMotion::BackstepMotion()
{
	motion_ = MotionManager::GetMotion("Backstep");
}

void BackstepMotion::Init(PlayerBody* human)
{
	isInit_ = false;
	isPlay_ = false;
	isCanChangeMotion_ = false;
	step_ = 0;

	curRots_.resize(human->GetPartsSize());
	endRots_.resize(human->GetPartsSize());
	curWeaponRots_.resize(human->GetWeaponPartsSize());
	endWeaponRots_.resize(human->GetWeaponPartsSize());

	moveEase_.SetEaseTimer(15);
	moveEase_.SetPowNum(2);
}
void BackstepMotion::Update(PlayerBody* human)
{
	if (isPlay_ == false)
	{
		isPlay_ = true;
	}

	ease_.Update();

	if (isInit_ == false)
	{
		BaseInit(human);
		CurrentStepInit(human);
		isInit_ = true;
	}
	BasePrevUpdate(human);
	CurrentStepUpdate(human);
	BasePostUpdate(human);
}

void BackstepMotion::CurrentStepInit(PlayerBody* human)
{
	if (step_ == 0)
	{
		Step0Init(human);
	}
}
void BackstepMotion::CurrentStepUpdate(PlayerBody* human)
{
	if (step_ == 0)
	{
		Step0Update(human);
	}
	else if (step_ == 1)
	{
		Step1Update(human);
	}
	else if (step_ == 2)
	{
		Step2Update(human);
	}
}

void BackstepMotion::Step0Init(PlayerBody* human)
{
	moveEase_.Reset();

	step_ = 0;

	// �U�����[�V�����Ői�ދ����̌v�Z
	human->parent->CalcFrontVec();
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(-human->parent->frontVec_, 15);

	// ���݂̍��W���擾
	startPos_ = human->pos;
	endPos_ = startPos_ - human->parent->frontVec_.Norm() * length_;

	up_ = 1;
	down_ = -0.5;
}
void BackstepMotion::Step0Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.Interpolation(0.f, up_);
	human->pos = moveEase_.InOut(startPos_, endPos_);
	human->parent->moveVel = endPos_ - startPos_;

	moveEase_.Update();

	if (ease_.GetisEnd() == true)
	{
	}
}
void BackstepMotion::Step1Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.Interpolation(up_, down_);
	human->pos = moveEase_.InOut(startPos_, endPos_);
	human->parent->moveVel = endPos_ - startPos_;

	moveEase_.Update();

	if (ease_.GetisEnd() == true)
	{
		isCanChangeMotion_ = true;
	}
}
void BackstepMotion::Step2Update(PlayerBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.Interpolation(down_, 0.f);

	if (ease_.GetisEnd() == true)
	{
		isCanChangeMotion_ = false;
	}
}
