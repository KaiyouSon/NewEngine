#include "Tree.h"

Tree::Tree() :
	mTree(std::make_unique<Object3D>()),
	mBranch(std::make_unique<Object3D>())
{
	mTree->SetModel(ModelManager::GetModel("Tree"));
	mBranch->SetModel(ModelManager::GetModel("Branch"));

	mCollider.isActive = false;
}

void Tree::Init()
{
	mTree->SetisShadow(false, true);
	mBranch->SetisShadow(false, true);
	mBranch->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Branch"));
}

void Tree::Update()
{
	if (mCollider.isActive == true)
	{
		ColliderDrawer::GetInstance()->Bind(&mCollider);
	}

	mTree->Update(&mParent);
	mBranch->Update(&mParent);
}

void Tree::DrawModel()
{
	mTree->Draw();
	mBranch->Draw();
}

void Tree::SetParent(const Transform& parent)
{
	mParent = parent;
	mParent.Update();
}

void Tree::SetCollider(const SphereCollider collider)
{
	mCollider = collider;
	mCollider.isActive = true;
}

void Tree::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mTree->SetGraphicsPipeline(graphicsPipeline);
	mBranch->SetGraphicsPipeline(graphicsPipeline);
}

Vec3 Tree::GetPos()
{
	return mParent.pos;
}

SphereCollider Tree::GetCollider()
{
	return mCollider;
}
