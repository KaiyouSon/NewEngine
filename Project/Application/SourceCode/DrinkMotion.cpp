#include "DrinkMotion.h"
#include "HumanoidBody.h"
#include "PlayerBody.h"
#include "CollisionManager.h"
#include "EffectManager.h"

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
	CurrentStepUpdate(human);
	BasePostUpdate(human);
}

void DrinkMotion::CurrentStepInit(HumanoidBody* human)
{
}
void DrinkMotion::CurrentStepUpdate(HumanoidBody* human)
{
	if (step_ == 2)
	{
		if (ease_.GetisEnd() == true)
		{
			Player* player = static_cast<Player*>(human->iParent);
			Vec3 pos = player->GetPos() + Vec3::down * 2;

			player->Recovery();
			EffectManager::GetInstance()->GeneratePlayerRecoveryEffect(pos);
		}
	}

}
