#include "BackstepMotion.h"
#include "HumanoidBody.h"
#include "Player.h"
#include "CollisionManager.h"

void BackstepMotion::Init(HumanoidBody* human)
{
	isInit_ = false;
	isPlay_ = false;
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

void BackstepMotion::Step0Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(5);
	ease_.SetPowNum(2);
	ease_.Reset();

	moveEase_.Reset();

	step_ = 0;

	// 攻撃モーションで進む距離の計算
	human->CalcFrontVec();
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(-human->frontVec, 15);

	// 現在の座標を取得
	startPos_ = human->pos;
	endPos_ = startPos_ - human->frontVec.Norm() * length_;

	Step0RotsInit(human);
	curRots_ = human->CalcCurRots();
}
void BackstepMotion::Step0RotsInit(HumanoidBody* human)
{
	// 各部位の角度の初期化
	endRots_[(uint32_t)PartID::Head] = Radian(Vec3(10, -15, 5));
	endRots_[(uint32_t)PartID::Body] = Radian(Vec3(-25, 30, -25));
	endRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(30, 30, 50));
	endRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-60, -20, 0));
	endRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(25, 0, -15));
	endRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-40, 0, 0));
	endRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(-65, 55, 0));
	endRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(60, 0, 20));
	endRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-40, -20, 0));
	endRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(70, 0, 0));
}
void BackstepMotion::Step0Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.InOut(0.f, up_);
	for (uint32_t i = (uint32_t)PartID::Head; i < curRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.InOut(curRots_[i], endRots_[i]);
	}

	human->pos = moveEase_.InOut(startPos_, endPos_);
	human->parent->moveVel = endPos_ - startPos_;

	ease_.Update();
	moveEase_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_ = 1;
		isInit_ = false;
	}
}

void BackstepMotion::Step1Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(10);
	ease_.SetPowNum(2);
	ease_.Reset();

	endPos_ = startPos_ - human->frontVec.Norm() * length_;

	Step1RotsInit(human);
	curRots_ = human->CalcCurRots();
}
void BackstepMotion::Step1RotsInit(HumanoidBody* human)
{
	// 各部位の角度の初期化
	endRots_[(uint32_t)PartID::Head] = Radian(Vec3(-5, -30, -5));
	endRots_[(uint32_t)PartID::Body] = Radian(Vec3(10, 30, 10));
	endRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(40, 30, 50));
	endRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-60, 15, 0));
	endRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(55, 0, -20));
	endRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-80, 0, -15));
	endRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(-45, 30, 0));
	endRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(105, -10, 20));
	endRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-90, -50, 0));
	endRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(90, 20, 0));
}
void BackstepMotion::Step1Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.InOut(up_, down_);
	for (uint32_t i = (uint32_t)PartID::Head; i < curRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.InOut(curRots_[i], endRots_[i]);
	}

	human->pos = moveEase_.InOut(startPos_, endPos_);
	human->parent->moveVel = endPos_ - startPos_;

	ease_.Update();
	moveEase_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_ = 2;
		isInit_ = false;
		ease_.Reset();
	}
}

void BackstepMotion::Step2Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(30);
	ease_.SetPowNum(2);
	ease_.Reset();

	Step2RotsInit(human);
	curRots_ = human->CalcCurRots();
}
void BackstepMotion::Step2RotsInit(HumanoidBody* human)
{
}
void BackstepMotion::Step2Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.InOut(down_, 0.f);
	for (uint32_t i = (uint32_t)PartID::Head; i < curRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.Out(curRots_[i], 0);
	}

	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_ = 0;
		isInit_ = false;
		isPlay_ = false;
		ease_.Reset();
	}
}
