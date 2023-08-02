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

	mStartBodyY = human->GetPart(PartID::Body)->pos.y;
	switch (mStep)
	{
	case 0:
		mEndBodyY = -0.3f;
		break;
	case 1:
		mEndBodyY = -0.28f;
		break;
	case 2:
		mEndBodyY = -1.2f;
		boss->GetWeapon()->SetisCalcCollider(true);
		boss->mDamage = 30.f;
		break;
	case 3:
		SettingMovePrame(human, 5, 15, 2);
		mEndBodyY = -1.1f;
		break;
	case 4:
		mEndBodyY = -1.0f;
		boss->GetWeapon()->SetisCalcCollider(false);
		boss->mDamage = 0.f;
		break;
	case 5:
		mEndBodyY = -1.3f;
		boss->GetWeapon()->SetisCalcCollider(true);
		boss->mDamage = 30.f;
		break;
	case 6:
		SettingMovePrame(human, 5, 15, 2);
		mEndBodyY = -0.37f;
		break;
	case 7:
		mEndBodyY = -1.1f;
		boss->GetWeapon()->SetisCalcCollider(false);
		boss->mDamage = 0.f;
		break;
	case 8:
		mEndBodyY = -0.95f;
		break;
	case 9:
		mEndBodyY = -0.95f;
		boss->GetWeapon()->SetisCalcCollider(true);
		boss->mDamage = 50.f;
		break;
	case 10:
		SettingMovePrame(human, 8, 10, 2);
		mEndBodyY = -1.5f;
		break;
	case 11:
		mEndBodyY = -0.95f;
		boss->GetWeapon()->SetisCalcCollider(false);
		boss->mDamage = 0.f;
		break;
	default:
		mEndBodyY = 0;
	}
}
void BossAttack3Motion::CurrentStepUpdate(HumanoidBody* human)
{
	human->GetPart(PartID::Body)->pos.y = mEase.Interpolation(mStartBodyY, mEndBodyY);

	Boss* boss = static_cast<Boss*>(human->iParent);
	if (mStep == 3 || mStep == 6 || mStep == 10)
	{
		human->pos = mMoveEase.InOut(mStartPos, mEndPos);
		mMoveEase.Update();
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

	mMoveEase.SetEaseTimer(easeTimer);
	mMoveEase.SetPowNum(powNum);
	mMoveEase.Reset();
	boss->CalcFrontVec();

	// 攻撃モーションで進む距離の計算
	//length_ = 5;

	// 現在の座標を取得
	mStartPos = human->pos;
	mEndPos = mStartPos + boss->mFrontVec.Norm() * dis;
}

