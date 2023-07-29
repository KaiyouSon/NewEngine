#include "Boss.h"
#include "Sword.h"
#include "Player.h"

Boss::Boss() :
	boss_(std::make_unique<BossBody>()),
	weapon(std::make_unique<Sword>())
{
}
void Boss::Init()
{
	boss_->Init();
	boss_->pos = Vec3(0, 7.125f, 20.f);
	boss_->scale = 1.5f;
	boss_->rot.y = Radian(180);

	boss_->SetWeapon(weapon.get(), WeaponPartID::Right);
	boss_->iParent = this;
	boss_->parent = static_cast<Boss*>(boss_->iParent);

	// HPƒQ[ƒW
	hpGaugeParam_.CalcRate(2560.f, 2560.f);

	coolTimer_.SetLimitTimer(120);

	isDamage_ = false;
	damageCoolTimer_.SetLimitTimer(60);
}
void Boss::Update()
{
	ColliderUpdate();

	MotionUpdate();

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

	weapon->ColliderUpdate();
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
			motionNum_ = Random::Range(1, 3);
			motionNum_ = 3;
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
	return weapon.get();
}
