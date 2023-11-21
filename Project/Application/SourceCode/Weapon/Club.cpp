#include "Club.h"
#include "MalletMotion.h"

Club::Club()
{
	weapon = std::make_unique<Object3D>();
	weapon->SetModel(ModelManager::GetModel("Club"));
	weapon->SetisShadow(false, false);

	// モーション生成
	motion = std::make_unique<MalletMotion>();

	mLocalPos = Vec3(0.f, -1.5f, 0.f);

	Init();
}

void Club::Init()
{
	mWeakAttackDamage = 85.f;
	mHeavyAttackDamage = 185.f;
	mBackAttackDamage = 65.f;
	mRollAttackDamage = 125.f;
}

void Club::Update(Transform* parent)
{
	weapon->Update(parent);

	collider.startPos;
}

void Club::DrawModel()
{
	weapon->Draw("Object3D");
}

Vec3 Club::GetTipPos()
{
	return Vec3();
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
		Vec3 upVec = weapon->GetTransform().GetWorldMat().GetYAxis().Norm();

		collider.startPos = weapon->GetWorldPos() - upVec * 1.5f;
		collider.endPos = weapon->GetWorldPos() + upVec * 6.5f;
		collider.radius = 1.5f;

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
		mDamage = Random::RangeF(mWeakAttackDamage - range, mWeakAttackDamage + range);
		break;

	case AttackType::Heavy:
		range = 10.f;
		mDamage = Random::RangeF(mHeavyAttackDamage - range, mHeavyAttackDamage + range) * mChargeRate;
		break;

	case AttackType::Back:
		range = 5.f;
		mDamage = Random::RangeF(mBackAttackDamage - range, mBackAttackDamage + range);
		break;

	case AttackType::Roll:
		range = 10.f;
		mDamage = Random::RangeF(mRollAttackDamage - range, mRollAttackDamage + range);
		break;

	default:
		mDamage = 0.f;
		break;
	}
}

