#include "DrinkMotion.h"
#include "HumanoidBody.h"
#include "PlayerBody.h"
#include "CollisionManager.h"
#include "EffectManager.h"

DrinkMotion::DrinkMotion()
{
	mMotion = MotionManager::GetMotion("Drink");
}

void DrinkMotion::Init(HumanoidBody* human)
{
	mIsInit = false;
	mIsPlay = false;
	mIsCanChangeMotion = false;
	mStep = 0;

	mCurRots.resize(human->GetPartsSize());
	mEndRots.resize(human->GetPartsSize());
	mCurWeaponPoses.resize(human->GetWeaponPartsSize());
	mEndWeaponPoses.resize(human->GetWeaponPartsSize());
	mCurWeaponRots.resize(human->GetWeaponPartsSize());
	mEndWeaponRots.resize(human->GetWeaponPartsSize());

	if (human->GetPart(PartID::Body)->rot.x >= Radian(360))
	{
		human->GetPart(PartID::Body)->rot.x -= Radian(360);
	}
}
void DrinkMotion::Update(HumanoidBody* human)
{
	if (mIsPlay == false)
	{
		mIsPlay = true;
	}

	mEase.Update();

	if (mIsInit == false)
	{
		BaseInit(human);
		//CurrentStepInit(human);
		mIsInit = true;
	}
	BasePrevUpdate(human);
	CurrentStepUpdate(human);
	BasePostUpdate(human);
}

void DrinkMotion::CurrentStepInit(HumanoidBody* human)
{
}
void DrinkMotion::CurrentStepUpdate(HumanoidBody* human)
{
	Player* player = static_cast<Player*>(human->iParent);
	if (player->GetBottleNum() <= 0)
	{
		return;
	}

	if (mStep == 2)
	{
		if (mEase.GetisEnd() == true)
		{
			Vec3 pos = player->GetPos() + Vec3::down * 2;

			player->Recovery();
			SoundManager::Play("RecoverySE");
			EffectManager::GetInstance()->GeneratePlayerRecoveryEffect(pos);
		}
	}

}
