#include "Sword.h"
#include "MalletMotion.h"
#include "EffectManager.h"

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
	mTrajectory->PrevUpdate();

	weapon->SetParent(parent);
	weapon->Update();

	Vec3 yAxis = weapon->GetTransform()->GetWorldMat().GetYAxis();
	Vec3 zAxis = weapon->GetTransform()->GetWorldMat().GetZAxis();
	Vec3 pos = weapon->GetTransform()->GetWorldMat().GetTrans();

	mTrajectory->SetTargetPos(
		pos + zAxis.Norm() * 2.f,
		pos + zAxis.Norm() * 10.f);

	// エフェクト生成
	EffectManager::GetInstance()->
		ExecuteBossAttackTrajectoryEffect(
			mIsActiveTrajectory,
			mTrajectory->GetDownPos(),
			mTrajectory->GetTopPos());

	mTrajectory->PostUpdate();

	collider.startPos;
}

void Sword::DrawModel()
{
	weapon->Draw("Object3D");

	if (mIsActiveTrajectory == true)
	{
		mTrajectory->Draw();
	}
}

Vec3 Sword::GetTipPos()
{
	Vec3 zAxis = weapon->GetTransform()->GetWorldMat().GetZAxis();
	Vec3 pos = weapon->GetTransform()->GetWorldMat().GetTrans();

	return pos + zAxis.Norm() * 10.f;
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
		Vec3 zAxis = weapon->GetTransform()->GetWorldMat().GetZAxis();

		collider.startPos = weapon->GetWorldPos() - zAxis.Norm() * 8.f;
		collider.endPos = weapon->GetWorldPos() + zAxis.Norm() * 8.f;
		collider.radius = 3.f;
	}
}

void Sword::CalcDamage()
{
}
