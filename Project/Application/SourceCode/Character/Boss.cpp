#include "Boss.h"
#include "Sword.h"
#include "Player.h"

Boss::Boss() :
	mBoss(std::make_unique<BossBody>()),
	mWeapon(std::make_unique<Sword>())
{
}
void Boss::Init()
{
	mBoss->Init();
	mBoss->pos = Vec3(0, 7.125f, 400.f);
	mBoss->scale = 1.5f;
	mRotY = Radian(180);

	mBoss->SetWeapon(mWeapon.get(), WeaponPartID::Right);
	mBoss->iParent = this;
	mBoss->parent = static_cast<Boss*>(mBoss->iParent);

	// HP繧ｲ繝ｼ繧ｸ
	mHpGaugeParam.CalcRate(2560.f, 2560.f);

	mCoolTimer.SetLimitTimer(120);

	mIsDamage = false;
	mDamageCoolTimer.SetLimitTimer(40);

	mWeapon->weapon->isUseDissolve = true;
	mWeapon->weapon->colorPower = 5;
	mWeapon->weapon->dissolveColor = Color(255, 30, 0, 255);

	mIsAlive = true;
	mIsDissolve = false;
	mIsFight = false;
}
void Boss::Update()
{
	float disToPlayer = Vec3::Distance(mPlayer->GetPos(), mBoss->pos);
	if (disToPlayer <= 150.f)
	{
		if (mIsFight == false)
		{
			SoundManager::Play("BattleBGM", true);
			SoundManager::SetVolume("BattleBGM", 0);
			mIsFight = true;
		}
	}

	if (mIsAlive == false)
	{
		for (uint32_t i = 1; i < mBoss->mParts.size(); i++)
		{
			mBoss->mParts[i]->dissolve += 0.005f;
			mBoss->mParts[i]->dissolve = Min(mBoss->mParts[i]->dissolve, 2.f);
		}

		mWeapon->weapon->dissolve += 0.005f;
		mWeapon->weapon->dissolve = Min(mWeapon->weapon->dissolve, 2.f);

		if (mBoss->mParts[1]->dissolve >= 0.5f)
		{
			if (mIsDissolve == false)
			{
				mIsDissolve = true;
			}
		}
	}
	else
	{
		if (mIsFight == true)
		{
			MotionUpdate();
		}
	}

	if (mIsDamage == true)
	{
		mDamageCoolTimer.Update();
		if (mDamageCoolTimer == true)
		{
			mDamageCoolTimer.Reset();
			mIsDamage = false;
		}
	}

	// HP繧ｲ繝ｼ繧ｸ
	mHpGaugeParam.CalcRate(mHpGaugeParam.value, 2560.f);

	mBoss->rot.y = mRotY;
	mBoss->Update();

	ColliderUpdate();

}
void Boss::DrawModel()
{
	mBoss->DrawModel();
}

void Boss::CalcFrontVec()
{
	Vec3 v = mPlayer->GetPos() - mBoss->pos;
	v.y = 0;

	mFrontVec = v.Norm();
	mRotY = atan2f(mFrontVec.x, mFrontVec.z);
}
void Boss::ColliderUpdate()
{
	mCollider.startPos = mBoss->pos - Vec3(0.f, 7.f, 0.f);
	mCollider.endPos = mBoss->pos + Vec3(0.f, 7.f, 0.f);
	mCollider.radius = 5.f;

	mBodyCollider.centerPos = mBoss->pos;
	mBodyCollider.size = Vec3(2, 4, 2);
	mBodyCollider.CalcPoints();

	mWeapon->ColliderUpdate();
}

void Boss::MotionUpdate()
{
	if (mBoss->GetisPlayMotion() == false)
	{
		CalcFrontVec();

		mMotionNum = 0;
		mCoolTimer.Update();
		if (mCoolTimer == true)
		{
			mMotionNum = Random::Range(2, 3);
		}
	}
	if (mMotionNum != 0)
	{
		mCoolTimer.Reset();

		switch (mMotionNum)
		{
		case 1:
			break;
		case 2:
			mBoss->Attack2Motion();
			break;
		case 3:
			mBoss->Attack3Motion();
		}
	}
}

void Boss::Damage(const float damage)
{
	mHpGaugeParam.value -= damage;
	mIsDamage = true;

	if (mHpGaugeParam.value <= 0)
	{
		mIsAlive = false;
	}
}

void Boss::SetPlayer(Player* player)
{
	mPlayer = player;
}
void Boss::SetisDamage(const bool isDamage)
{
	mIsDamage = isDamage;
}

void Boss::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	for (uint32_t i = 0; i < mBoss->mParts.size(); i++)
	{
		mBoss->mParts[i]->SetGraphicsPipeline(graphicsPipeline);
	}
	mWeapon->SetGraphicsPipeline(graphicsPipeline);
}

CapsuleCollider Boss::GetCollider()
{
	return mCollider;
}
CubeCollider Boss::GetBodyCollider()
{
	return mBodyCollider;
}
GaugeParam Boss::GetHpGaugeParam()
{
	return mHpGaugeParam;
}
Vec3 Boss::GetPos()
{
	return mBoss->pos;
}
bool Boss::GetisDamage()
{
	return mIsDamage;
}
Weapon* Boss::GetWeapon()
{
	return mWeapon.get();
}
float Boss::GetDamage()
{
	return mDamage;
}
bool Boss::GetisAlive()
{
	return mIsAlive;
}
bool Boss::GetisDissolve()
{
	return mIsDissolve;
}
bool Boss::GetisFight()
{
	return mIsFight;
}
