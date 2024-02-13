#include "Wall.h"

Wall::Wall() :
	mWall(std::make_unique<Object3D>())
{
	mCollider.isActive = false;
	mType = FieldObjectType::Wall;
}
void Wall::Init()
{
	mWall->SetisShadow(true, true);
}
void Wall::Update()
{
	if (mCollider.isActive == true)
	{
		ColliderDrawer::GetInstance()->Bind(&mCollider);
	}

	mWall->SetParent(&mParent);
	mWall->Update();
}
void Wall::Draw(const bool isDrawDepth)
{
	if (isDrawDepth == false)
	{
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));
	}
	else
	{
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	}

	mWall->Draw("Object3D");
}

// セッター
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

// ゲッター
Vec3 Wall::GetPos()
{
	return mParent.pos;
}
CapsuleCollider Wall::GetCollider()
{
	return mCollider;
}
