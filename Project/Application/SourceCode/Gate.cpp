#include "Gate.h"

Gate::Gate() :
	mGateLeft(std::make_unique<Object3D>()),
	mGateRight(std::make_unique<Object3D>())
{
	mGateLeft->SetModel(ModelManager::GetModel("WallGate"));
	mGateRight->SetModel(ModelManager::GetModel("WallGate"));
	mLeftCollider.isActive = false;
	mRightCollider.isActive = false;
}

void Gate::Init()
{
	mGateLeft->SetisShadow(false, true);
	mGateRight->SetisShadow(false, true);
	mGateLeft->isLighting = true;
	mGateRight->isLighting = true;
}

void Gate::Update()
{
	ColliderDrawer::GetInstance()->Bind(&mLeftCollider);
	ColliderDrawer::GetInstance()->Bind(&mRightCollider);

	mGateLeft->Update();
	mGateRight->Update();
}

void Gate::DrawModel()
{
	mGateLeft->Draw();
	mGateRight->Draw();
}

void Gate::SetLeftTransform(const Transform& transform)
{
	mGateLeft->pos = transform.pos;
	mGateLeft->scale = transform.scale;
	mGateLeft->rot = transform.rot;
}

void Gate::SetRightTransform(const Transform& transform)
{
	mGateRight->pos = transform.pos;
	mGateRight->scale = transform.scale;
	mGateRight->rot = transform.rot;
}

void Gate::SetLeftCollider(const CapsuleCollider collider)
{
	mLeftCollider = collider;
	mLeftCollider.isActive = true;
}

void Gate::SetRightCollider(const CapsuleCollider collider)
{
	mRightCollider = collider;
	mRightCollider.isActive = true;
}

void Gate::SetCenterPos(const Vec3 pos)
{
	centerPos = pos;
}

void Gate::SetLeftRot(const Vec3 rot)
{
	mGateLeft->rot = rot;
}

void Gate::SetRightRot(const Vec3 rot)
{
	mGateRight->rot = rot;
}

CapsuleCollider Gate::GetLeftCollider()
{
	return mLeftCollider;
}

CapsuleCollider Gate::GetRightCollider()
{
	return mRightCollider;
}

Vec3 Gate::GetCenterPos()
{
	return centerPos;
}
