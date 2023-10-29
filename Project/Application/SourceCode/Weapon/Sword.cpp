#include "Sword.h"
#include "MalletMotion.h"

Sword::Sword() :
	mTrajectory(std::make_unique<Trajectory>())
{
	weapon = std::make_unique<Object3D>();
	weapon->SetModel(ModelManager::GetModel("Sword"));
	weapon->SetisShadow(false, true);

	// 槌のモーション
	motion = std::make_unique<MalletMotion>();

	mLocalPos = Vec3(0.f, -1.5f, 0.f);

	mIsCalcCollider = false;
}

void Sword::Init()
{
	Vec3 zAxis = weapon->GetTransform().GetWorldMat().GetZAxis();
	Vec3 pos = weapon->GetWorldPos();

	mTrajectory->pos[Trajectory::LD] = pos - zAxis.Norm() * 8.f;
	mTrajectory->pos[Trajectory::LT] = pos + zAxis.Norm() * 8.f;

	mTrajectory->pos[Trajectory::RD] = mTrajectory->pos[Trajectory::LD];
	mTrajectory->pos[Trajectory::RT] = mTrajectory->pos[Trajectory::LT];
}

void Sword::Update(Transform* parent)
{
	Vec3 yAxis = weapon->GetTransform().GetWorldMat().GetYAxis();
	Vec3 zAxis = weapon->GetTransform().GetWorldMat().GetZAxis();
	Vec3 pos = weapon->GetWorldPos();

	mTrajectory->moveSpeed = 1.f;
	mTrajectory->pos[Trajectory::LD] = pos + yAxis.Norm() * 2.f + zAxis.Norm() * 2.f;
	mTrajectory->pos[Trajectory::LT] = pos + yAxis.Norm() * 2.f + zAxis.Norm() * 10.f;

	weapon->Update(parent);
	mTrajectory->Update();

	collider.startPos;
}

void Sword::DrawModel()
{
	weapon->Draw();
	mTrajectory->Draw();
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
