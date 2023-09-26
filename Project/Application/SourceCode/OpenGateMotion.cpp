#include "OpenGateMotion.h"
#include "HumanoidBody.h"
#include "PlayerBody.h"
#include "CollisionManager.h"
#include "EffectManager.h"

OpenGateMotion::OpenGateMotion()
{
	mMotion = MotionManager::GetMotion("OpenGate");
}

void OpenGateMotion::Init(HumanoidBody* human)
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
void OpenGateMotion::Update(HumanoidBody* human)
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

void OpenGateMotion::CurrentStepInit(HumanoidBody* human)
{
	Player* player = static_cast<Player*>(human->iParent);


	Vec3 rot = Vec3(0, 0, 0);
	player->SetRot(rot);
	player->CalcFrontVec();

	mStartPos = human->pos;
	mEndPos = mStartPos + Vec3::front * 20.f;

	uint32_t  maxTimer = 0;
	for (uint32_t i = 0; i < mMotion->data.size(); i++)
	{
		maxTimer += mMotion->data[i].ease.GetEaseTimer();
	}
	mMoveEase.SetEaseTimer(maxTimer);
	mMoveEase.Reset();

}
void OpenGateMotion::CurrentStepUpdate(HumanoidBody* human)
{
	human->pos = mMoveEase.InOut(mStartPos, mEndPos);
	mMoveEase.Update();
}
