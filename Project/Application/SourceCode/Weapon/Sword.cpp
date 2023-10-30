#include "Sword.h"
#include "MalletMotion.h"

Sword::Sword() :
	mTrajectory(std::make_unique<Trajectory>())
{
	mTrajectory->SetTexture(TextureManager::GetTexture("Trajectory"));

	weapon = std::make_unique<Object3D>();
	weapon->SetModel(ModelManager::GetModel("Sword"));
	weapon->SetisShadow(false, true);

	// 槌のモーション
	motion = std::make_unique<MalletMotion>();

	mLocalPos = Vec3(0.f, -1.5f, 0.f);

	mIsCalcCollider = false;
	mTrajectory->color = Color(0x890000);

}

void Sword::Init()
{
}

void Sword::Update(Transform* parent)
{
	weapon->Update(parent);

	Vec3 yAxis = weapon->GetTransform().GetWorldMat().GetYAxis();
	Vec3 zAxis = weapon->GetTransform().GetWorldMat().GetZAxis();
	Vec3 pos = weapon->GetTransform().GetWorldMat().GetTrans();

	mTrajectory->pos[Trajectory::LD] = pos + zAxis.Norm() * 4.f;
	mTrajectory->pos[Trajectory::LT] = pos + zAxis.Norm() * 10.f;
	if (mIsActiveTrajectory == false)
	{
		mTrajectory->pos[Trajectory::RD] = mTrajectory->pos[Trajectory::LD];
		mTrajectory->pos[Trajectory::RT] = mTrajectory->pos[Trajectory::LT];
	}

	mTrajectory->Update();

	collider.startPos;
}

void Sword::DrawModel()
{
	weapon->Draw();

	if (mIsActiveTrajectory == true)
	{
		mTrajectory->Draw();
	}
}

void Sword::ColliderUpdate()
{
	if (mIsCalcCollider == false)
	{
		collider.startPos = Vec3(0, -10000, 0);
		collider.endPos = Vec3(0, -10000, 0);
		collider.radius = 0.f;
	}
	else
	{
		Vec3 zAxis = weapon->GetTransform().GetWorldMat().GetZAxis();

		collider.startPos = weapon->GetWorldPos() - zAxis.Norm() * 8.f;
		collider.endPos = weapon->GetWorldPos() + zAxis.Norm() * 8.f;
		collider.radius = 3.f;
	}
}

void Sword::CalcDamage()
{
}
