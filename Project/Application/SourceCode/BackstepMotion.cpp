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

	if (isInit_ == false)
	{
		CurrentMotionInit(human);
		isInit_ = true;
	}
	CurrentMotionUpdate(human);

	/*if (step_ == 0)
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
	}*/
}

void BackstepMotion::CurrentMotionInit(HumanoidBody* human)
{
	curRots_ = human->CalcCurRots();
	MotionData current = motion_->data[step_];
	for (uint32_t i = 0; i < current.endRots.size(); i++)
	{
		endRots_[i] = current.endRots[i];
	}
	ease_ = current.ease;


	if (step_ == 0)
	{
		Step0Init(human);
	}
	else if (step_ == 1)
	{
		Step1Init(human);
	}
	else if (step_ == 2)
	{
		Step2Init(human);
	}
}
void BackstepMotion::CurrentMotionUpdate(HumanoidBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Body; i < curRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.Interpolation(curRots_[i], endRots_[i]);
	}
	ease_.Update();

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
	curRots_ = human->CalcCurRots();

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
		step_ = 1;
		isInit_ = false;
	}
}

void BackstepMotion::Step1Init(HumanoidBody* human)
{
	//ease_.SetEaseTimer(10);
	//ease_.SetPowNum(2);
	//

	endPos_ = startPos_ - human->frontVec.Norm() * length_;



	//Step1RotsInit(human);
	curRots_ = human->CalcCurRots();
}
void BackstepMotion::Step1Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.Interpolation(up_, down_);
	human->pos = moveEase_.InOut(startPos_, endPos_);
	human->parent->moveVel = endPos_ - startPos_;


	moveEase_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_ = 2;
		isInit_ = false;
		isCanChangeMotion_ = true;

	}
}

void BackstepMotion::Step2Init(HumanoidBody* human)
{
}
void BackstepMotion::Step2Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.Interpolation(down_, 0.f);

	if (ease_.GetisEnd() == true)
	{
		step_ = 0;
		isInit_ = false;
		isPlay_ = false;
		isCanChangeMotion_ = false;
	}
}
