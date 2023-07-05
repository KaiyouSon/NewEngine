#include "RollMotion.h"
#include "HumanoidBody.h"
#include "CollisionManager.h"

void RollMotion::Init(HumanoidBody* human)
{
	isInit_ = false;
	isPlay_ = false;
	isCanChangeMotion_ = false;
	step_ = 0;

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
	else if (step_ == 3)
	{
		if (isInit_ == false)
		{
			Step3Init(human);
			isInit_ = true;
		}
		Step3Update(human);
	}
}

void RollMotion::Step0Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(10);
	ease_.SetPowNum(2);
	ease_.Reset();

	moveEase_.Reset();

	// 攻撃モーションで進む距離の計算
	human->CalcFrontVec();
	length_ = CollisionManager::GetInstance()->CalcPlayerDisToFront(-human->frontVec, 30);

	// 現在の座標を取得
	startPos_ = human->pos;
	endPos_ = startPos_ + human->frontVec.Norm() * length_;

	Step0RotsInit(human);
	curRots_ = human->CalcCurRots();
}
void RollMotion::Step0RotsInit(HumanoidBody* human)
{
	// 各部位の角度の初期化
	endRots_[(uint32_t)PartID::Head] = Radian(Vec3(25, 0, 0));
	endRots_[(uint32_t)PartID::Body] = Radian(Vec3(130, 0, 0));
	endRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(-80, 30, 30));
	endRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-50, 0, 0));
	endRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(-80, 0, -40));
	endRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-60, 0, 0));
	endRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(-110, 0, 30));
	endRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(120, 0, 0));
	endRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-50, 0, -30));
	endRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(30, 0, 0));
}
void RollMotion::Step0Update(HumanoidBody* human)
{
	for (uint32_t i = (uint32_t)PartID::Head; i < curRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.In(curRots_[i], endRots_[i]);
	}

	human->pos = moveEase_.InOut(startPos_, endPos_);
	human->parent->moveVel = endPos_ - startPos_;

	ease_.Update();
	moveEase_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_ = 1;
		isInit_ = false;
		ease_.Reset();
	}
}

void RollMotion::Step1Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(10);
	ease_.SetPowNum(2);
	ease_.Reset();

	Step1RotsInit(human);
	curRots_ = human->CalcCurRots();
}
void RollMotion::Step1RotsInit(HumanoidBody* human)
{
	// 各部位の角度の初期化
	endRots_[(uint32_t)PartID::Head] = Radian(Vec3(35, 0, 0));
	endRots_[(uint32_t)PartID::Body] = Radian(Vec3(300, 0, 0));
	endRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(-30, 30, 15));
	endRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-65, 0, 0));
	endRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(-80, 0, -60));
	endRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-70, 30, 0));
	endRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(-90, 0, 30));
	endRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(110, 0, 0));
	endRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-80, 0, -30));
	endRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(20, 0, 0));
}
void RollMotion::Step1Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.In(0, -3);
	for (uint32_t i = (uint32_t)PartID::Head; i < curRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.Lerp(curRots_[i], endRots_[i]);
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

void RollMotion::Step2Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(10);
	ease_.SetPowNum(2);
	ease_.Reset();

	Step2RotsInit(human);
	curRots_ = human->CalcCurRots();
}
void RollMotion::Step2RotsInit(HumanoidBody* human)
{
	// 各部位の角度の初期化
	endRots_[(uint32_t)PartID::Head] = Radian(Vec3(10, 0, 0));
	endRots_[(uint32_t)PartID::Body] = Radian(Vec3(390, 0, 0));
	endRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(10, 30, 15));
	endRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-50, 0, 0));
	endRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(20, -15, -15));
	endRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-90, 0, 0));
	endRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(-90, 0, 30));
	endRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(70, 15, 0));
	endRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-90, 0, -30));
	endRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(70, -15, 0));
}
void RollMotion::Step2Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.Lerp(-3.f, -0.7f);
	for (uint32_t i = (uint32_t)PartID::Head; i < curRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.Lerp(curRots_[i], endRots_[i]);
	}

	human->pos = moveEase_.InOut(startPos_, endPos_);
	human->parent->moveVel = endPos_ - startPos_;

	ease_.Update();
	moveEase_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_ = 3;
		isInit_ = false;
		isCanChangeMotion_ = true;
		ease_.Reset();
	}
}

void RollMotion::Step3Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(5);
	ease_.SetPowNum(2);
	ease_.Reset();

	curRots_ = human->CalcCurRots();
}
void RollMotion::Step3RotsInit(HumanoidBody* human)
{
}
void RollMotion::Step3Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.Out(-0.7f, 0.f);
	for (uint32_t i = (uint32_t)PartID::Head; i < curRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.Out(curRots_[i], 0);
		if (i == (uint32_t)PartID::Body)
		{
			human->GetPart((PartID)i)->rot.x = ease_.Out(curRots_[i].x, Radian(360));
		}
	}

	human->pos = moveEase_.InOut(startPos_, endPos_);
	human->parent->moveVel = endPos_ - startPos_;

	ease_.Update();
	moveEase_.Update();

	if (ease_.GetisEnd() == true)
	{
		human->GetPart(PartID::Body)->rot.x = 0;
		step_ = 0;
		isInit_ = false;
		isPlay_ = false;
		isCanChangeMotion_ = false;
		ease_.Reset();
	}
}
