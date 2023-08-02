#include "BossBody.h"
#include "Boss.h"

BossBody::BossBody() :
	mGrabAttackMotion(std::make_unique<BossGrabAttackMotion>()),
	mAttack2Motion(std::make_unique<BossAttack2Motion>()),
	mAttack3Motion(std::make_unique<BossAttack3Motion>())
{
}

void BossBody::Init()
{
	BaseInit();
	mGrabAttackMotion->Init(this);
	mAttack2Motion->Init(this);
	mAttack3Motion->Init(this);

	for (uint32_t i = 1; i < mParts.size(); i++)
	{
		mParts[i]->isUseDissolve = true;
		mParts[i]->colorPower = 5;
		mParts[i]->dissolveColor = Color(255, 30, 0, 255);
	}
}

void BossBody::Update()
{
	BaseUpdate();
}

void BossBody::DrawModel()
{
	BaseDrawModel();
	DrawWeapon(WeaponPartID::Right);
}

void BossBody::GrabAttackMotion()
{
	mGrabAttackMotion->Update(this);
	mIsPlayMotion = mGrabAttackMotion->GetisPlay();
}
void BossBody::Attack2Motion()
{
	mAttack2Motion->Update(this);
	mIsPlayMotion = mAttack2Motion->GetisPlay();
}
void BossBody::Attack3Motion()
{
	mAttack3Motion->Update(this);
	mIsPlayMotion = mAttack3Motion->GetisPlay();
}

bool BossBody::GetisPlayMotion()
{
	return mIsPlayMotion;
}
