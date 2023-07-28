#include "DrinkMotion.h"
#include "HumanoidBody.h"
#include "PlayerBody.h"
#include "CollisionManager.h"

DrinkMotion::DrinkMotion()
{
	motion_ = MotionManager::GetMotion("Drink");
}

void DrinkMotion::Init(HumanoidBody* human)
{
	isInit_ = false;
	isPlay_ = false;
	isCanChangeMotion_ = false;
	step_ = 0;

	human->pos.y = 4.5f;
	curRots_.resize(human->GetPartsSize());
	endRots_.resize(human->GetPartsSize());
	curWeaponPoses_.resize(human->GetWeaponPartsSize());
	endWeaponPoses_.resize(human->GetWeaponPartsSize());
	curWeaponRots_.resize(human->GetWeaponPartsSize());
	endWeaponRots_.resize(human->GetWeaponPartsSize());

	if (human->GetPart(PartID::Body)->rot.x >= Radian(360))
	{
		human->GetPart(PartID::Body)->rot.x -= Radian(360);
	}
}
void DrinkMotion::Update(HumanoidBody* human)
{
	if (isPlay_ == false)
	{
		isPlay_ = true;
	}

	ease_.Update();

	if (isInit_ == false)
	{
		BaseInit(human);
		//CurrentStepInit(human);
		isInit_ = true;
	}
	BasePrevUpdate(human);
	//CurrentStepUpdate(human);
	BasePostUpdate(human);
}

//void DrinkMotion::CurrentStepInit(HumanoidBody* human)
//{
//	if (step_ == 0)
//	{
//		Step0Init(human);
//	}
//}
//void DrinkMotion::CurrentStepUpdate(HumanoidBody* human)
//{
//	if (step_ == 0)
//	{
//		Step0Update(human);
//	}
//	else if (step_ == 1)
//	{
//		Step1Update(human);
//	}
//	else if (step_ == 2)
//	{
//		Step2Update(human);
//	}
//	else if (step_ == 3)
//	{
//		Step3Update(human);
//	}
//}

//void DrinkMotion::Step0Init(HumanoidBody* human)
//{
//	Player* player = static_cast<Player*>(human->iParent);
//
//	moveEase_.Reset();
//
//	// UŒ‚ƒ‚[ƒVƒ‡ƒ“‚Åi‚Þ‹——£‚ÌŒvŽZ
//	player->CalcFrontVec();
//	length_ = 30;// CollisionManager::GetInstance()->CalcPlayerDisToFront(human->parent->frontVec_, 30);
//
//	// Œ»Ý‚ÌÀ•W‚ðŽæ“¾
//	startPos_ = human->pos;
//	endPos_ = startPos_ + player->frontVec_.Norm() * length_;
//
//	// “ü—Í‚µ‚½Œã‚Ì‰ñ“]Šp‚ðŽæ“¾
//	rotY_ = atan2f(player->frontVec_.x, player->frontVec_.z);
//}
//void DrinkMotion::Step0Update(HumanoidBody* human)
//{
//	Player* player = static_cast<Player*>(human->iParent);
//
//	human->pos = moveEase_.InOut(startPos_, endPos_);
//	human->rot.y = rotY_;
//	player->moveVel = endPos_ - startPos_;
//
//	moveEase_.Update();
//}
//void DrinkMotion::Step1Update(HumanoidBody* human)
//{
//	Player* player = static_cast<Player*>(human->iParent);
//
//	human->GetPart(PartID::Body)->pos.y = ease_.In(0, -3);
//	human->pos = moveEase_.InOut(startPos_, endPos_);
//	player->moveVel = endPos_ - startPos_;
//
//	moveEase_.Update();
//
//	if (ease_.GetisEnd() == true)
//	{
//		//human->GetPart(PartID::Body)->rot.x = 30;
//	}
//}
//void DrinkMotion::Step2Update(HumanoidBody* human)
//{
//	Player* player = static_cast<Player*>(human->iParent);
//
//	human->GetPart(PartID::Body)->pos.y = ease_.Lerp(-3.f, -0.7f);
//	human->pos = moveEase_.InOut(startPos_, endPos_);
//	player->moveVel = endPos_ - startPos_;
//
//	moveEase_.Update();
//
//	if (ease_.GetisEnd() == true)
//	{
//		isCanChangeMotion_ = true;
//	}
//}
//void DrinkMotion::Step3Update(HumanoidBody* human)
//{
//	Player* player = static_cast<Player*>(human->iParent);
//
//	human->GetPart(PartID::Body)->pos.y = ease_.Out(-0.7f, 0.f);
//	human->pos = moveEase_.InOut(startPos_, endPos_);
//	player->moveVel = endPos_ - startPos_;
//
//	moveEase_.Update();
//
//	if (ease_.GetisEnd() == true)
//	{
//		human->GetPart(PartID::Body)->rot.x = 0;
//	}
//}
