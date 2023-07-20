#include "BossAttack2Motion.h"
#include "HumanoidBody.h"
#include "Boss.h"

BossAttack2Motion::BossAttack2Motion()
{
	motion_ = MotionManager::GetMotion("BossAttack2");
}

void BossAttack2Motion::Init(HumanoidBody* human)
{
	curRots_.resize(human->GetPartsSize());
	endRots_.resize(human->GetPartsSize());
	curWeaponPoses_.resize(human->GetWeaponPartsSize());
	endWeaponPoses_.resize(human->GetWeaponPartsSize());
	curWeaponRots_.resize(human->GetWeaponPartsSize());
	endWeaponRots_.resize(human->GetWeaponPartsSize());
}
void BossAttack2Motion::Update(HumanoidBody* human)
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

void BossAttack2Motion::CurrentStepInit(HumanoidBody* human)
{
	Boss* boss = static_cast<Boss*>(human->iParent);

	startBodyY_ = human->GetPart(PartID::Body)->pos.y;
	switch (step_)
	{
	case 0:
		endBodyY_ = -0.45f;
		break;
	case 1:
		endBodyY_ = -0.42f;
		break;
	case 2:
		endBodyY_ = -1.8f;
		break;
	case 3:
		moveEase_.SetEaseTimer(15);
		moveEase_.SetPowNum(2);
		moveEase_.Reset();
		boss->CalcFrontVec();

		// 攻撃モーションで進む距離の計算
		length_ = 5;

		// 現在の座標を取得
		startPos_ = human->pos;
		endPos_ = startPos_ + boss->frontVec_.Norm() * length_;

		endBodyY_ = -1.1f;
		break;
	case 4:
		endBodyY_ = -1.0f;
		break;
	case 5:
		endBodyY_ = -1.3f;
		break;
	case 6:
		moveEase_.SetEaseTimer(15);
		moveEase_.SetPowNum(2);
		moveEase_.Reset();
		boss->CalcFrontVec();

		// 攻撃モーションで進む距離の計算
		length_ = 5;

		// 現在の座標を取得
		startPos_ = human->pos;
		endPos_ = startPos_ + boss->frontVec_.Norm() * length_;

		endBodyY_ = -0.37f;
		break;
	case 7:
		endBodyY_ = -1.1f;
		break;
	case 8:
	case 9:
		endBodyY_ = -0.95f;
		break;
	case 10:
		moveEase_.SetEaseTimer(10);
		moveEase_.SetPowNum(2);
		moveEase_.Reset();
		boss->CalcFrontVec();

		// 攻撃モーションで進む距離の計算
		length_ = 8;

		// 現在の座標を取得
		startPos_ = human->pos;
		endPos_ = startPos_ + boss->frontVec_.Norm() * length_;

		endBodyY_ = -1.5f;
		break;
	case 11:
		endBodyY_ = -0.95f;
		break;
	case 12:
		endBodyY_ = 0.f;
		break;
	default:
		endBodyY_ = 0;
	}
}
void BossAttack2Motion::CurrentStepUpdate(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.Interpolation(startBodyY_, endBodyY_);

	//Boss* boss = static_cast<Boss*>(human->iParent);
	//if (step_ == 3 || step_ == 6 || step_ == 10)
	//{
	//	human->pos = moveEase_.InOut(startPos_, endPos_);
	//	moveEase_.Update();
	//}
}

