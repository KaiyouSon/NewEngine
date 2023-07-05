#include "Club.h"
#include "MalletMotion.h"

Club::Club()
{
	weapon = std::make_unique<Object3D>();
	weapon->SetModel(ModelManager::GetModel("Club"));

	// ’Æ‚Ìƒ‚[ƒVƒ‡ƒ“
	motion = std::make_unique<MalletMotion>();

	localPos_ = Vec3(0.f, -1.5f, 0.f);
	localRot_ = Radian(Vec3(90.f, 0.f, 0.f));
}

void Club::Init()
{
	motion->Init();
}

void Club::Update(Transform* parent)
{
	weapon->Update(parent);

	ColliderUpdate(motion->GetisCalcCollider());

	collider.startPos;
}

void Club::DrawModel()
{
	weapon->Draw();
}

void Club::ColliderUpdate(bool isCalc)
{
	if (isCalc == false)
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