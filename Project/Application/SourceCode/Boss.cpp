#include "Boss.h"

Boss::Boss() :
	boss_(std::make_unique<BossBody>())
{
}

void Boss::Init()
{
	boss_->Init();
	boss_->pos = Vec3(0, 7.125f, 20.f);
	boss_->scale = 1.5f;
	boss_->rot.y = Radian(180);

	// HPƒQ[ƒW
	hpGaugeParam_.CalcRate(2560.f, 2560.f);
}

void Boss::Update()
{
	ColliderUpdate();

	// HPƒQ[ƒW
	hpGaugeParam_.CalcRate(hpGaugeParam_.value, 2560.f);

	boss_->Update();
}

void Boss::DrawModel()
{
	boss_->DrawModel();
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
