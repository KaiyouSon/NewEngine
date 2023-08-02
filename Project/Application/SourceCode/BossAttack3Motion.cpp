#include "BossAttack3Motion.h"
#include "HumanoidBody.h"
#include "Boss.h"

BossAttack3Motion::BossAttack3Motion()
{
	mMotion = MotionManager::GetMotion("BossAttack3");
}

void BossAttack3Motion::Init(HumanoidBody* human)
{
	mCurRots.resize(human->GetPartsSize());
	mEndRots.resize(human->GetPartsSize());
	mCurWeaponPoses.resize(human->GetWeaponPartsSize());
	mEndWeaponPoses.resize(human->GetWeaponPartsSize());
	mCurWeaponRots.resize(human->GetWeaponPartsSize());
	mEndWeaponRots.resize(human->GetWeaponPartsSize());
}
void BossAttack3Motion::Update(HumanoidBody* human)
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

void BossAttack3Motion::CurrentStepInit(HumanoidBody* human)
{
	Boss* boss = static_cast<Boss*>(human->iParent);

	startBodyY_ = human->GetPart(PartID::Body)->pos.y;
	switch (mStep)
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
		boss->mDamage = 30.f;
		break;
	case 3:
		SettingMovePrame(human, 5, 15, 2);
		endBodyY_ = -1.1f;
		break;
	case 4:
		endBodyY_ = -1.0f;
		boss->GetWeapon()->SetisCalcCollider(false);
		boss->mDamage = 0.f;
		break;
	case 5:
		endBodyY_ = -1.3f;
		boss->GetWeapon()->SetisCalcCollider(true);
		boss->mDamage = 30.f;
		break;
	case 6:
		SettingMovePrame(human, 5, 15, 2);
		endBodyY_ = -0.37f;
		break;
	case 7:
		endBodyY_ = -1.1f;
		boss->GetWeapon()->SetisCalcCollider(false);
		boss->mDamage = 0.f;
		break;
	case 8:
		endBodyY_ = -0.95f;
		break;
	case 9:
		endBodyY_ = -0.95f;
		boss->GetWeapon()->SetisCalcCollider(true);
		boss->mDamage = 50.f;
		break;
	case 10:
		SettingMovePrame(human, 8, 10, 2);
		endBodyY_ = -1.5f;
		break;
	case 11:
		endBodyY_ = -0.95f;
		boss->GetWeapon()->SetisCalcCollider(false);
		boss->mDamage = 0.f;
		break;
	default:
		endBodyY_ = 0;
	}
}
void BossAttack3Motion::CurrentStepUpdate(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = mEase.Interpolation(startBodyY_, endBodyY_);

	Boss* boss = static_cast<Boss*>(human->iParent);
	if (mStep == 3 || mStep == 6 || mStep == 10)
	{
		human->pos = moveEase_.InOut(startPos_, endPos_);
		moveEase_.Update();
	}

	if (mStep == 3 || mStep == 6)
	{
		if (mEase.GetTimer() == 4)
		{
			SoundManager::Play("BossAttackSE");
		}
	}
	else if (mStep == 9)
	{
		if (mEase.GetisEnd() == true)
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
	endPos_ = startPos_ + boss->mFrontVec.Norm() * dis;
}

