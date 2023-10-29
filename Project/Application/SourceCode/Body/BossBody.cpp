#include "BossBody.h"
#include "Boss.h"

BossBody::BossBody() :
	mAttack2Motion(std::make_unique<BossAttack2Motion>()),
	mAttack3Motion(std::make_unique<BossAttack3Motion>())
{
}

void BossBody::Init()
{
	BaseInit();
	mAttack2Motion->Init(this);
	mAttack3Motion->Init(this);

	for (uint32_t i = 1; i < mParts.size(); i++)
	{
		mParts[i]->isUseDissolve = true;
		mParts[i]->colorPower = 5;
		mParts[i]->dissolveColor = Color(255, 30, 0, 255);

		mParts[i]->SetisShadow(false, true);

		// 頭と体以外のパーティの色を黒にする
		if (i != (uint32_t)PartID::Head &&
			i != (uint32_t)PartID::Body &&
			i != (uint32_t)PartID::LeftArm &&
			i != (uint32_t)PartID::RightArm)
		{
			mParts[i]->color = Color::black;
		}
	}

	mParts[(uint32_t)PartID::Head]->SetModel(ModelManager::GetModel("BossHead"));
	mParts[(uint32_t)PartID::Body]->SetModel(ModelManager::GetModel("BossBody"));
	mParts[(uint32_t)PartID::LeftArm]->SetModel(ModelManager::GetModel("BossLeftArm"));
	mParts[(uint32_t)PartID::RightArm]->SetModel(ModelManager::GetModel("BossRightArm"));
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
