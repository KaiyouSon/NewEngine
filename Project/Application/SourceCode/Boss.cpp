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
}
void Boss::Update()
{
	CalcFrontVec();
	CalcRotY();
	ColliderUpdate();

	// HPƒQ[ƒW
	hpGaugeParam_.CalcRate(hpGaugeParam_.value, 2560.f);

	//boss_->rot.y = rotY_;
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

	frontVec_ = Vec3::back;

	int a = 0;
}
void Boss::CalcRotY()
{
	rotY_ = atan2f(frontVec_.x, frontVec_.z);
}
void Boss::ColliderUpdate()
{
	collider_.startPos = boss_->pos - Vec3(0.f, 2.5f, 0.f);
	collider_.endPos = boss_->pos + Vec3(0.f, 2.5f, 0.f);
	collider_.radius = 2.5f;

	bodyCollider_.centerPos = boss_->pos;
	bodyCollider_.size = Vec3(2, 4, 2);
	bodyCollider_.CalcPoints();
}

void Boss::Damage(const float damage)
{
	hpGaugeParam_.value -= damage;
}

void Boss::SetPlayer(Player* player)
{
	player_ = player;
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
