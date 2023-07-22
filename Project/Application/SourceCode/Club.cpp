#include "Club.h"
#include "MalletMotion.h"

Club::Club()
{
	weapon = std::make_unique<Object3D>();
	weapon->SetModel(ModelManager::GetModel("Club"));

	// í∆ÇÃÉÇÅ[ÉVÉáÉì
	motion = std::make_unique<MalletMotion>();

	localPos_ = Vec3(0.f, -1.5f, 0.f);

	Init();
}

void Club::Init()
{
	weakAttackDamage_ = 85.f;
	heavyAttackDamage_ = 185.f;
	backAttackDamage_ = 65.f;
	rollAttackDamage_ = 125.f;
}

void Club::Update(Transform* parent)
{
	weapon->Update(parent);

	collider.startPos;
}

void Club::DrawModel()
{
	weapon->Draw();
}

void Club::ColliderUpdate()
{
	if (motion->GetisCalcCollider() == false)
	{
		collider.startPos = Vec3(0, -10000, 0);
		collider.endPos = Vec3(0, -10000, 0);
		collider.radius = 0.f;
	}
	else
	{
		Vec3 upVec = weapon->GetTransform().GetWorldMat().GetYAxis();

		collider.startPos = weapon->GetWorldPos();
		collider.endPos = weapon->GetWorldPos() + upVec.Norm() * 4.f;
		collider.radius = 0.5f;

		CalcDamage();
	}
}

void Club::CalcDamage()
{
	float range = 0.f;
	switch (motion->GetAttackType())
	{
	case AttackType::Weak:
		range = 5.f;
		damage_ = Random::RangeF(weakAttackDamage_ - range, weakAttackDamage_ + range);
		break;

	case AttackType::Heavy:
		range = 10.f;
		damage_ = Random::RangeF(heavyAttackDamage_ - range, heavyAttackDamage_ + range);
		break;

	case AttackType::Back:
		range = 5.f;
		damage_ = Random::RangeF(backAttackDamage_ - range, backAttackDamage_ + range);
		break;

	case AttackType::Roll:
		range = 10.f;
		damage_ = Random::RangeF(rollAttackDamage_ - range, rollAttackDamage_ + range);
		break;

	default:
		damage_ = 0.f;
		break;
	}
}
