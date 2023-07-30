#include "BossAttack3Motion.h"
#include "HumanoidBody.h"
#include "Boss.h"

BossAttack3Motion::BossAttack3Motion()
{
	motion_ = MotionManager::GetMotion("BossAttack3");
}

void BossAttack3Motion::Init(HumanoidBody* human)
{
	curRots_.resize(human->GetPartsSize());
	endRots_.resize(human->GetPartsSize());
	curWeaponPoses_.resize(human->GetWeaponPartsSize());
	endWeaponPoses_.resize(human->GetWeaponPartsSize());
	curWeaponRots_.resize(human->GetWeaponPartsSize());
	endWeaponRots_.resize(human->GetWeaponPartsSize());
}
void BossAttack3Motion::Update(HumanoidBody* human)
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

void BossAttack3Motion::CurrentStepInit(HumanoidBody* human)
{
	Boss* boss = static_cast<Boss*>(human->iParent);

	startBodyY_ = human->GetPart(PartID::Body)->pos.y;
	switch (step_)
	{
	case 0:
		endBodyY_ = -0.3f;
		break;
	case 1:
		endBodyY_ = -0.28f;
		break;
	case 2:
		endBodyY_ = -1.2f;
		boss->GetWeapon()->SetisCalcCollider(true);
		boss->damage_ = 30.f;
		break;
	case 3:
		SettingMovePrame(human, 5, 15, 2);
		endBodyY_ = -1.1f;
		break;
	case 4:
		endBodyY_ = -1.0f;
		boss->GetWeapon()->SetisCalcCollider(false);
		boss->damage_ = 0.f;
		break;
	case 5:
		endBodyY_ = -1.3f;
		boss->GetWeapon()->SetisCalcCollider(true);
		boss->damage_ = 30.f;
		break;
	case 6:
		SettingMovePrame(human, 5, 15, 2);
		endBodyY_ = -0.37f;
		break;
	case 7:
		endBodyY_ = -1.1f;
		boss->GetWeapon()->SetisCalcCollider(false);
		boss->damage_ = 0.f;
		break;
	case 8:
		endBodyY_ = -0.95f;
		break;
	case 9:
		endBodyY_ = -0.95f;
		boss->GetWeapon()->SetisCalcCollider(true);
		boss->damage_ = 50.f;
		break;
	case 10:
		SettingMovePrame(human, 8, 10, 2);
		endBodyY_ = -1.5f;
		break;
	case 11:
		endBodyY_ = -0.95f;
		boss->GetWeapon()->SetisCalcCollider(false);
		boss->damage_ = 0.f;
		break;
	default:
		endBodyY_ = 0;
	}
}
void BossAttack3Motion::CurrentStepUpdate(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.Interpolation(startBodyY_, endBodyY_);

	Boss* boss = static_cast<Boss*>(human->iParent);
	if (step_ == 3 || step_ == 6 || step_ == 10)
	{
		human->pos = moveEase_.InOut(startPos_, endPos_);
		moveEase_.Update();
	}

	if (step_ == 3 || step_ == 6)
	{
		if (ease_.GetTimer() == 4)
		{
			SoundManager::Play("BossAttackSE");
		}
	}
	else if (step_ == 9)
	{
		if (ease_.GetisEnd() == true)
		{
			SoundManager::Play("BossAttackSE");
		}
	}
}

void BossAttack3Motion::SettingMovePrame(HumanoidBody* human, const float dis, const uint32_t easeTimer, const float powNum)
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

