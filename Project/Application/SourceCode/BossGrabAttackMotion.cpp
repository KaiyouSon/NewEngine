#include "BossGrabAttackMotion.h"
#include "HumanoidBody.h"
#include "Boss.h"

BossGrabAttackMotion::BossGrabAttackMotion()
{
	motion_ = MotionManager::GetMotion("BossGrabAttack");
}

void BossGrabAttackMotion::Init(HumanoidBody* human)
{
	curRots_.resize(human->GetPartsSize());
	endRots_.resize(human->GetPartsSize());
	curWeaponPoses_.resize(human->GetWeaponPartsSize());
	endWeaponPoses_.resize(human->GetWeaponPartsSize());
	curWeaponRots_.resize(human->GetWeaponPartsSize());
	endWeaponRots_.resize(human->GetWeaponPartsSize());
}
void BossGrabAttackMotion::Update(HumanoidBody* human)
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

void BossGrabAttackMotion::CurrentStepInit(HumanoidBody* human)
{
	startBodyY_ = human->GetPart(PartID::Body)->pos.y;
	switch (step_)
	{
	case 0:
		SettingMovePrame(human, 3, 30, 2);
		endBodyY_ = -0.0f;
		break;
	case 1:
		endBodyY_ = -0.26f;
		break;
	case 2:
		endBodyY_ = -0.24f;
		break;
	case 3:
		SettingMovePrame(human, 12, 30, 3);
		endBodyY_ = -0.48f;
		break;
	case 4:
		endBodyY_ = -0.43f;
		break;
	default:
		endBodyY_ = 0;
	}
}
void BossGrabAttackMotion::CurrentStepUpdate(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = ease_.Interpolation(startBodyY_, endBodyY_);

	Boss* boss = static_cast<Boss*>(human->iParent);
	if ((step_ >= 0 && step_ <= 2) ||
		(step_ == 3))
	{
		human->pos = moveEase_.InOut(startPos_, endPos_);
		moveEase_.Update();
	}
}

void BossGrabAttackMotion::SettingMovePrame(HumanoidBody* human, const float dis, const uint32_t easeTimer, const float powNum)
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

