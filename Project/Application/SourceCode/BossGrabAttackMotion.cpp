#include "BossGrabAttackMotion.h"
#include "HumanoidBody.h"
#include "Boss.h"

BossGrabAttackMotion::BossGrabAttackMotion()
{
	mMotion = MotionManager::GetMotion("BossGrabAttack");
}

void BossGrabAttackMotion::Init(HumanoidBody* human)
{
	mCurRots.resize(human->GetPartsSize());
	mEndRots.resize(human->GetPartsSize());
	mCurWeaponPoses.resize(human->GetWeaponPartsSize());
	mEndWeaponPoses.resize(human->GetWeaponPartsSize());
	mCurWeaponRots.resize(human->GetWeaponPartsSize());
	mEndWeaponRots.resize(human->GetWeaponPartsSize());
}
void BossGrabAttackMotion::Update(HumanoidBody* human)
{
	if (mIsPlay == false)
	{
		mIsPlay = true;
	}

	mEase.Update();

	if (mIsInit == false)
	{
		BaseInit(human);
		CurrentStepInit(human);
		mIsInit = true;
	}
	BasePrevUpdate(human);
	CurrentStepUpdate(human);
	BasePostUpdate(human);
}

void BossGrabAttackMotion::CurrentStepInit(HumanoidBody* human)
{
	startBodyY_ = human->GetPart(PartID::Body)->pos.y;
	switch (mStep)
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
	human->GetPart(PartID::Body)->pos.y = mEase.Interpolation(startBodyY_, endBodyY_);

	Boss* boss = static_cast<Boss*>(human->iParent);
	if ((mStep >= 0 && mStep <= 2) ||
		(mStep == 3))
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
	endPos_ = startPos_ + boss->mFrontVec.Norm() * dis;
}

