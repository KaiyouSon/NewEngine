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
	startBodyY_ = human->GetPart(PartID::Body)->pos.y;
	switch (step_)
	{
	case 0:
		endBodyY_ = -0.9f;
		break;
	case 1:
		endBodyY_ = -1.1f;
		break;
	case 2:
		SettingMovePrame(human, 5, 30, 2);
		endBodyY_ = -0.85f;
		break;
	case 3:
		endBodyY_ = -1.0f;
		break;
	case 4:
		endBodyY_ = -1.3f;
		break;
	case 5:
		endBodyY_ = -0.55f;
		break;
	case 6:
		endBodyY_ = -0.5f;
		break;
	case 7:
	case 8:
		endBodyY_ = -0.6f;
		break;
	case 9:
		endBodyY_ = -0.55f;
		break;
	case 10:
		endBodyY_ = -0.85f;
		break;
	case 11:
		SettingMovePrame(human, 5, 30, 2);
		endBodyY_ = -1.3f;
		break;
	case 12:
		endBodyY_ = -1.25f;
		break;
	case 13:
		endBodyY_ = -1.3f;
		break;
	case 14:
		endBodyY_ = -0.6f;
		break;
	case 15:
		endBodyY_ = 0.0f;
		break;
	case 16:
		SettingMovePrame(human, 8, 20, 2);
		endBodyY_ = -1.75f;
		break;
	case 17:
		endBodyY_ = -0.7f;
		break;
	default:
		endBodyY_ = 0;
	}
}
void BossAttack2Motion::CurrentStepUpdate(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.Interpolation(startBodyY_, endBodyY_);

	Boss* boss = static_cast<Boss*>(human->iParent);
	if ((step_ >= 2 && step_ <= 4) ||
		(step_ >= 11 && step_ <= 13) ||
		(step_ == 16))
	{
		human->pos = moveEase_.InOut(startPos_, endPos_);
		moveEase_.Update();
	}
}

void BossAttack2Motion::SettingMovePrame(HumanoidBody* human, const float dis, const uint32_t easeTimer, const float powNum)
{
	Boss* boss = static_cast<Boss*>(human->iParent);

	moveEase_.SetEaseTimer(easeTimer);
	moveEase_.SetPowNum(powNum);
	moveEase_.Reset();
	boss->CalcFrontVec();

	// 攻撃モーションで進む距離の計算
	//length_ = 5;

	// 現在の座標を取得
	startPos_ = human->pos;
	endPos_ = startPos_ + boss->frontVec_.Norm() * dis;
}

