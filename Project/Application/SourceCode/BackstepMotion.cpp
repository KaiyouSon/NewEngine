#include "BackstepMotion.h"
#include "HumanoidBody.h"
#include "Player.h"
#include "CollisionManager.h"

BackstepMotion::BackstepMotion()
{
	motion_ = MotionManager::GetMotion("Backstep");
}

void BackstepMotion::Init(HumanoidBody* human)
{
	isInit_ = false;
	isPlay_ = false;
	isCanChangeMotion_ = false;
	step_ = 0;

	curRots_.resize(human->GetPartsSize());
	endRots_.resize(human->GetPartsSize());

	moveEase_.SetEaseTimer(15);
	moveEase_.SetPowNum(2);
}
void BackstepMotion::Update(HumanoidBody* human)
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

void BackstepMotion::CurrentStepInit(HumanoidBody* human)
{
	if (step_ == 0)
	{
		Step0Init(human);
	}
}
void BackstepMotion::CurrentStepUpdate(HumanoidBody* human)
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

void BackstepMotion::Step0Init(HumanoidBody* human)
{
	moveEase_.Reset();

	step_ = 0;

	// 攻撃モーションで進む距離の計算
	human->CalcFrontVec();
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(-human->frontVec, 15);

	// 現在の座標を取得
	startPos_ = human->pos;
	endPos_ = startPos_ - human->frontVec.Norm() * length_;

	up_ = 1;
	down_ = -0.5;
}
void BackstepMotion::Step0Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.Interpolation(0.f, up_);
	human->pos = moveEase_.InOut(startPos_, endPos_);
	human->parent->moveVel = endPos_ - startPos_;

	moveEase_.Update();

	if (ease_.GetisEnd() == true)
	{
	}
}
void BackstepMotion::Step1Update(HumanoidBody* human)
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
void BackstepMotion::Step2Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.Interpolation(down_, 0.f);

	if (ease_.GetisEnd() == true)
	{
		isCanChangeMotion_ = false;
	}
}
