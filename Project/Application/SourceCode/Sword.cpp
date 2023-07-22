#include "Sword.h"
#include "MalletMotion.h"

Sword::Sword()
{
	weapon = std::make_unique<Object3D>();
	weapon->SetModel(ModelManager::GetModel("Sword"));

	// í∆ÇÃÉÇÅ[ÉVÉáÉì
	motion = std::make_unique<MalletMotion>();

	localPos_ = Vec3(0.f, -1.5f, 0.f);
}

void Sword::Init()
{
}

void Sword::Update(Transform* parent)
{
	weapon->Update(parent);

	collider.startPos;
}

void Sword::DrawModel()
{
	weapon->Draw();
}

void Sword::ColliderUpdate()
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
	}
}

void Sword::CalcDamage()
{
}
