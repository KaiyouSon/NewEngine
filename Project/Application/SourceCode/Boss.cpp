#include "Boss.h"
#include "Sword.h"
#include "Player.h"

Boss::Boss() :
	boss_(std::make_unique<BossBody>()),
	weapon_(std::make_unique<Sword>())
{
}
void Boss::Init()
{
	boss_->Init();
	boss_->pos = Vec3(0, 7.125f, 20.f);
	boss_->scale = 1.5f;
	boss_->rot.y = Radian(180);

	boss_->SetWeapon(weapon_.get(), WeaponPartID::Right);
	boss_->iParent = this;
	boss_->parent = static_cast<Boss*>(boss_->iParent);

	// HPƒQ[ƒW
	hpGaugeParam_.CalcRate(2560.f, 2560.f);

	coolTimer_.SetLimitTimer(120);

	isDamage_ = false;
	damageCoolTimer_.SetLimitTimer(40);

	weapon_->weapon->isUseDissolve = true;
	weapon_->weapon->colorPower = 5;
	weapon_->weapon->dissolveColor = Color(255, 30, 0, 255);

	isAlive_ = true;
	isDissolve_ = false;
}
void Boss::Update()
{
	if (isAlive_ == false)
	{
		for (uint32_t i = 1; i < boss_->parts_.size(); i++)
		{
			boss_->parts_[i]->dissolve += 0.005f;
			boss_->parts_[i]->dissolve = Min(boss_->parts_[i]->dissolve, 2.f);
		}

		weapon_->weapon->dissolve += 0.005f;
		weapon_->weapon->dissolve = Min(weapon_->weapon->dissolve, 2.f);

		if (boss_->parts_[1]->dissolve >= 0.5f)
		{
			if (isDissolve_ == false)
			{
				isDissolve_ = true;
			}
		}
	}
	else
	{
		MotionUpdate();
	}


	if (isDamage_ == true)
	{
		damageCoolTimer_.Update(false);
		if (damageCoolTimer_.GetisTimeOut() == true)
		{
			damageCoolTimer_.Reset();
			isDamage_ = false;
		}
	}

	// HPƒQ[ƒW
	hpGaugeParam_.CalcRate(hpGaugeParam_.value, 2560.f);

	boss_->rot.y = rotY_;
	boss_->Update();

	ColliderUpdate();

}
void Boss::DrawModel()
{
	boss_->DrawModel();
}

void Boss::CalcFrontVec()
{
	Vec3 v = player_->GetPos() - boss_->pos;
	v.y = 0;

	frontVec_ = v.Norm();
	rotY_ = atan2f(frontVec_.x, frontVec_.z);

	//frontVec_ = Vec3::back;
}
void Boss::ColliderUpdate()
{
	collider_.startPos = boss_->pos - Vec3(0.f, 7.f, 0.f);
	collider_.endPos = boss_->pos + Vec3(0.f, 7.f, 0.f);
	collider_.radius = 5.f;

	bodyCollider_.centerPos = boss_->pos;
	bodyCollider_.size = Vec3(2, 4, 2);
	bodyCollider_.CalcPoints();

	weapon_->ColliderUpdate();
}

void Boss::MotionUpdate()
{
	if (boss_->GetisPlayMotion() == false)
	{
		CalcFrontVec();

		motionNum_ = 0;
		coolTimer_.Update(false);
		if (coolTimer_.GetisTimeOut() == true)
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
			boss_->GrabAttackMotion();
			break;
		case 2:
			boss_->Attack2Motion();
			break;
		case 3:
			boss_->Attack3Motion();
		}
	}
}

void Boss::Damage(const float damage)
{
	hpGaugeParam_.value -= damage;
	isDamage_ = true;

	if (hpGaugeParam_.value <= 0)
	{
		isAlive_ = false;
	}
}

void Boss::SetPlayer(Player* player)
{
	player_ = player;
}
void Boss::SetisDamage(const bool isDamage)
{
	isDamage_ = isDamage;
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
	return boss_->pos;
}
bool Boss::GetisDamage()
{
	return isDamage_;
}
Weapon* Boss::GetWeapon()
{
	return weapon_.get();
}
float Boss::GetDamage()
{
	return damage_;
}
bool Boss::GetisAlive()
{
	return isAlive_;
}
bool Boss::GetisDissolve()
{
	return isDissolve_;
}
