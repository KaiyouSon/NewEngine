#include "Boss.h"
#include "Sword.h"
#include "Player.h"

Boss::Boss() :
	mBoss(std::make_unique<BossBody>()),
	weapon_(std::make_unique<Sword>())
{
}
void Boss::Init()
{
	mBoss->Init();
	mBoss->pos = Vec3(0, 7.125f, 400.f);
	mBoss->scale = 1.5f;
	rotY_ = Radian(180);

	mBoss->SetWeapon(weapon_.get(), WeaponPartID::Right);
	mBoss->iParent = this;
	mBoss->parent = static_cast<Boss*>(mBoss->iParent);

	// HPƒQ[ƒW
	hpGaugeParam_.CalcRate(2560.f, 2560.f);

	coolTimer_.SetLimitTimer(120);

	ismDamage = false;
	damageCoolTimer_.SetLimitTimer(40);

	weapon_->weapon->isUseDissolve = true;
	weapon_->weapon->colorPower = 5;
	weapon_->weapon->dissolveColor = Color(255, 30, 0, 255);

	isAlive_ = true;
	isDissolve_ = false;
	isFight_ = false;
}
void Boss::Update()
{
	float disToPlayer = Vec3::Distance(mPlayer->GetPos(), mBoss->pos);
	if (disToPlayer <= 150.f)
	{
		if (isFight_ == false)
		{
			SoundManager::Play("BattleBGM", true);
			SoundManager::SetVolume("BattleBGM", 0);
			isFight_ = true;
		}
	}

	if (isAlive_ == false)
	{
		for (uint32_t i = 1; i < mBoss->parts_.size(); i++)
		{
			mBoss->parts_[i]->dissolve += 0.005f;
			mBoss->parts_[i]->dissolve = Min(mBoss->parts_[i]->dissolve, 2.f);
		}

		weapon_->weapon->dissolve += 0.005f;
		weapon_->weapon->dissolve = Min(weapon_->weapon->dissolve, 2.f);

		if (mBoss->parts_[1]->dissolve >= 0.5f)
		{
			if (isDissolve_ == false)
			{
				isDissolve_ = true;
			}
		}
	}
	else
	{
		if (isFight_ == true)
		{
			MotionUpdate();
		}
	}

	if (ismDamage == true)
	{
		damageCoolTimer_.Update();
		if (damageCoolTimer_ == true)
		{
			damageCoolTimer_.Reset();
			ismDamage = false;
		}
	}

	// HPƒQ[ƒW
	hpGaugeParam_.CalcRate(hpGaugeParam_.value, 2560.f);

	mBoss->rot.y = rotY_;
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

	frontVec_ = v.Norm();
	rotY_ = atan2f(frontVec_.x, frontVec_.z);

	//frontVec_ = Vec3::back;
}
void Boss::ColliderUpdate()
{
	collider_.startPos = mBoss->pos - Vec3(0.f, 7.f, 0.f);
	collider_.endPos = mBoss->pos + Vec3(0.f, 7.f, 0.f);
	collider_.radius = 5.f;

	bodyCollider_.centerPos = mBoss->pos;
	bodyCollider_.size = Vec3(2, 4, 2);
	bodyCollider_.CalcPoints();

	weapon_->ColliderUpdate();
}

void Boss::MotionUpdate()
{
	if (mBoss->GetisPlayMotion() == false)
	{
		CalcFrontVec();

		motionNum_ = 0;
		coolTimer_.Update();
		if (coolTimer_ == true)
		{
			motionNum_ = Random::Range(2, 3);
			//motionNum_ = 2;
		}
	}
	if (motionNum_ != 0)
	{
		coolTimer_.Reset();

		switch (motionNum_)
		{
		case 1:
			mBoss->GrabAttackMotion();
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
	hpGaugeParam_.value -= damage;
	ismDamage = true;

	if (hpGaugeParam_.value <= 0)
	{
		isAlive_ = false;
	}
}

void Boss::SetPlayer(Player* player)
{
	mPlayer = player;
}
void Boss::SetisDamage(const bool isDamage)
{
	ismDamage = isDamage;
}

CapsuleCollider Boss::GetCollider()
{
	return collider_;
}
CubeCollider Boss::GetBodyCollider()
{
	return bodyCollider_;
}
GaugeParam Boss::GetHpGaugeParam()
{
	return hpGaugeParam_;
}
Vec3 Boss::GetPos()
{
	return mBoss->pos;
}
bool Boss::GetisDamage()
{
	return ismDamage;
}
Weapon* Boss::GetWeapon()
{
	return weapon_.get();
}
float Boss::GetDamage()
{
	return mDamage;
}
bool Boss::GetisAlive()
{
	return isAlive_;
}
bool Boss::GetisDissolve()
{
	return isDissolve_;
}
