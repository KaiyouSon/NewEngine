#include "RollMotion.h"
#include "HumanoidBody.h"

void RollMotion::Init(HumanoidBody* human)
{
	isInit_ = false;
	isPlay_ = false;
	step_ = 0;

	curRots_.resize(human->GetPartsSize());
	endRots_.resize(human->GetPartsSize());
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
}

void RollMotion::Step0Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(10);
	ease_.SetPowNum(2);
	ease_.Reset();

	Step0RotsInit(human);
	curRots_ = human->CalcCurRots();
}
void RollMotion::Step0RotsInit(HumanoidBody* human)
{
	// äeïîà ÇÃäpìxÇÃèâä˙âª
	endRots_[(uint32_t)PartID::Head] = Radian(Vec3(25, 0, 0));
	endRots_[(uint32_t)PartID::Body] = Radian(Vec3(130, -80, -80));
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
		human->GetPart((PartID)i)->rot = ease_.InOut(curRots_[i], endRots_[i]);
	}

	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		step_ = 2;
		isInit_ = false;
		ease_.Reset();
	}
}
