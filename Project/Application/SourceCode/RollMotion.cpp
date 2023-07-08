#include "RollMotion.h"
#include "HumanoidBody.h"
#include "CollisionManager.h"

RollMotion::RollMotion()
{
	motion_ = MotionManager::GetMotion("Roll");
}

void RollMotion::Init(HumanoidBody* human)
{
	isInit_ = false;
	isPlay_ = false;
	isCanChangeMotion_ = false;
	step_ = 0;

	human->pos.y = 4.5f;
	curRots_.resize(human->GetPartsSize());
	endRots_.resize(human->GetPartsSize());

	moveEase_.SetEaseTimer(35);
	moveEase_.SetPowNum(3);
}
void RollMotion::Update(HumanoidBody* human)
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

void RollMotion::CurrentStepInit(HumanoidBody* human)
{
	if (step_ == 0)
	{
		Step0Init(human);
	}
}
void RollMotion::CurrentStepUpdate(HumanoidBody* human)
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
	else if (step_ == 3)
	{
		Step3Update(human);
	}
}

void RollMotion::Step0Init(HumanoidBody* human)
{
	moveEase_.Reset();

	// 攻撃モーションで進む距離の計算
	human->CalcFrontVec();
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(-human->frontVec, 30);

	// 現在の座標を取得
	startPos_ = human->pos;
	endPos_ = startPos_ + human->frontVec.Norm() * length_;
}
void RollMotion::Step0Update(HumanoidBody* human)
{
	human->pos = moveEase_.InOut(startPos_, endPos_);
	human->parent->moveVel = endPos_ - startPos_;

	moveEase_.Update();

	if (ease_.GetisEnd() == true)
	{
	}
}
void RollMotion::Step1Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.In(0, -3);
	human->pos = moveEase_.InOut(startPos_, endPos_);
	human->parent->moveVel = endPos_ - startPos_;

	moveEase_.Update();

	if (ease_.GetisEnd() == true)
	{
		//human->GetPart(PartID::Body)->rot.x = 30;
	}
}
void RollMotion::Step2Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.Lerp(-3.f, -0.7f);
	human->pos = moveEase_.InOut(startPos_, endPos_);
	human->parent->moveVel = endPos_ - startPos_;

	moveEase_.Update();

	if (ease_.GetisEnd() == true)
	{
		isCanChangeMotion_ = true;
	}
}
void RollMotion::Step3Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.Out(-0.7f, 0.f);
	human->pos = moveEase_.InOut(startPos_, endPos_);
	human->parent->moveVel = endPos_ - startPos_;

	moveEase_.Update();

	if (ease_.GetisEnd() == true)
	{
		human->GetPart(PartID::Body)->rot.x = 0;
	}
}
