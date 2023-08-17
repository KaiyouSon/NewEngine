#include "Coffin.h"

Coffin::Coffin() :
	mCoffinTop(std::make_unique<Object3D>()),
	mCoffinBottom(std::make_unique<Object3D>())
{
	mCoffinTop->SetModel(ModelManager::GetModel("CoffinTop"));
	mCoffinBottom->SetModel(ModelManager::GetModel("CoffinBottom"));

	mBottomCollider.isActive = false;
}

void Coffin::Init()
{
	mCoffinTop->SetisShadow(false, true);
	mCoffinBottom->SetisShadow(false, true);
	mCoffinTop->isLighting = true;
	mCoffinBottom->isLighting = true;
}

void Coffin::Update()
{
	ColliderDrawer::GetInstance()->Bind(&mBottomCollider);

	mCoffinTop->Update(&mParent);
	mCoffinBottom->Update(&mParent);
}

void Coffin::DrawModel()
{
	mCoffinTop->Draw();
	mCoffinBottom->Draw();
}

void Coffin::SetParent(const Transform parent)
{
	mParent = parent;
	mParent.Update();
}

void Coffin::SetTopPos(const Vec3 pos)
{
	mCoffinTop->pos = pos;
}

void Coffin::SetBottomPos(const Vec3 pos)
{
	mCoffinBottom->pos = pos;
}

void Coffin::SetTopRot(const Vec3 rot)
{
	mCoffinTop->rot = rot;
}

void Coffin::SetBottomRot(const Vec3 rot)
{
	mCoffinBottom->rot = rot;
}

void Coffin::SetBottomCollider(const CubeCollider& collider)
{
	mBottomCollider = collider;
	mBottomCollider.CalcPoints();
	mBottomCollider.isActive = true;
}

CubeCollider Coffin::GetBottomCollider()
{
	return mBottomCollider;
}

Vec3 Coffin::GetPos()
{
	return mParent.pos;
}
