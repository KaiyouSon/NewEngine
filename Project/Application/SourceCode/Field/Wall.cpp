#include "Wall.h"

Wall::Wall() :
	mWall(std::make_unique<Object3D>())
{
	mCollider.isActive = false;
}

void Wall::Init()
{
	mWall->SetisShadow(false, true);
}

void Wall::Update()
{
	if (mCollider.isActive == true)
	{
		ColliderDrawer::GetInstance()->Bind(&mCollider);
	}

	mWall->Update(&mParent);
}

void Wall::DrawModel()
{
	mWall->Draw();
}

void Wall::SetParent(const Transform parent)
{
	mParent = parent;
	mParent.Update();
}

void Wall::SetCollider(const CapsuleCollider collider)
{
	mCollider = collider;
	mCollider.isActive = true;
}

void Wall::SetModel(Model* model)
{
	mWall->SetModel(model);
}

void Wall::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mWall->SetGraphicsPipeline(graphicsPipeline);
}

Vec3 Wall::GetPos()
{
	return mParent.pos;
}

CapsuleCollider Wall::GetCollider()
{
	return mCollider;
}