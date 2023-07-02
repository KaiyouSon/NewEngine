#include "Boss.h"

Boss::Boss() :
	boss_(std::make_unique<Object3D>())
{
	boss_->SetModel(ModelManager::GetModel("Capsule"));
}

void Boss::Init()
{
	boss_->pos = Vec3(0, 5.f, 20.f);
	boss_->scale = 2.5f;

	// HPƒQ[ƒW
	hpGaugeParam_.CalcRate(2560.f, 2560.f);
}

void Boss::Update()
{
	ColliderUpdate();

	// HPƒQ[ƒW
	hpGaugeParam_.CalcRate(hpGaugeParam_.value_, 2560.f);

	boss_->Update();
}

void Boss::DrawModel()
{
	boss_->Draw();
}

void Boss::ColliderUpdate()
{
	collider_.startPos = boss_->pos - Vec3(0.f, 2.5f, 0.f);
	collider_.endPos = boss_->pos + Vec3(0.f, 2.5f, 0.f);
	collider_.radius = 2.5f;
}

void Boss::Damage(const float damage)
{
	hpGaugeParam_.value_ -= damage;
}

CapsuleCollider Boss::GetCollider()
{
	return collider_;
}

GaugeParam Boss::GetHpGaugeParam()
{
	return hpGaugeParam_;
}
