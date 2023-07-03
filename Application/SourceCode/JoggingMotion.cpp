#include "JoggingMotion.h"
#include "HumanoidBody.h"

JoggingMotion::JoggingMotion()
{
}

void JoggingMotion::Init(HumanoidBody* human)
{
	ease_.SetEaseTimer(20);
	ease_.SetPowNum(2);

	RotsInit(human);
}

void JoggingMotion::Update(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->rot.x = Radian(5);
	human->GetPart(PartID::RightArm)->rot.x = Radian(5);
	human->GetPart(PartID::LeftArm)->rot.x = Radian(-5);

	for (uint32_t i = (uint32_t)PartID::Head; i < startRots_.size(); i++)
	{
		human->GetPart((PartID)i)->rot = ease_.InOut(startRots_[i], endRots_[i]);
	}

	ease_.Update();

	if (ease_.GetisEnd() == true)
	{
		ReverceRots();
		ease_.Reset();
	}

}

void JoggingMotion::ReverceRots()
{
	std::vector<Vec3> temp;
	temp.resize(startRots_.size());

	for (uint32_t i = 0; i < startRots_.size(); i++)
	{
		// スタートを格納する
		temp[i] = startRots_[i];

		// エンドをスタートに上書きする
		startRots_[i] = endRots_[i];

		//格納したスタートをエンドに上書きする
		endRots_[i] = temp[i];
	}
}

void JoggingMotion::RotsInit(HumanoidBody* human)
{
	startRots_.resize(human->GetPartsSize());
	endRots_.resize(human->GetPartsSize());

	startRots_[(uint32_t)PartID::Head] = Radian(Vec3(0, -2, 0));
	startRots_[(uint32_t)PartID::Body] = Radian(Vec3(5, 10, 0));
	startRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(35, 0, 5));
	startRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-70, 0, 0));
	startRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(-15, 0, -5));
	startRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-50, 0, 0));
	startRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(-50, 0, 0));
	startRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(30, 0, 0));
	startRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(30, 0, 0));
	startRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(30, 0, 0));

	endRots_[(uint32_t)PartID::Head] = Radian(Vec3(0, 2, 0));
	endRots_[(uint32_t)PartID::Body] = Radian(Vec3(5, -10, 0));
	endRots_[(uint32_t)PartID::RightArm] = Radian(Vec3(-15, 0, 5));
	endRots_[(uint32_t)PartID::RightHand] = Radian(Vec3(-50, 0, 0));
	endRots_[(uint32_t)PartID::LeftArm] = Radian(Vec3(35, 0, -5));
	endRots_[(uint32_t)PartID::LeftHand] = Radian(Vec3(-70, 0, 0));
	endRots_[(uint32_t)PartID::RightThigh] = Radian(Vec3(30, 0, 0));
	endRots_[(uint32_t)PartID::RightLeg] = Radian(Vec3(10, 0, 0));
	endRots_[(uint32_t)PartID::LeftThigh] = Radian(Vec3(-50, 0, 0));
	endRots_[(uint32_t)PartID::LeftLeg] = Radian(Vec3(10, 0, 0));
}