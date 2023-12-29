#include "Tree.h"

Tree::Tree() :
	mTree(std::make_unique<Object3D>()),
	mBranch(std::make_unique<Object3D>())
{
	mTree->SetModel(ModelManager::GetModel("Tree"));
	mBranch->SetModel(ModelManager::GetModel("Branch"));

	mCollider.isActive = false;
	mType = FieldObjectType::Tree;
}

void Tree::Init()
{
	mTree->SetisShadow(true, true);
	mBranch->SetisShadow(true, true);
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

void Tree::Draw(const bool isDrawDepth)
{
	if (isDrawDepth == false)
	{
		mTree->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));
		mBranch->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Branch"));
	}
	else
	{
		mTree->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
		mBranch->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("BranchWriteNone"));
	}

	mTree->Draw("Object3D");
	mBranch->Draw("Object3D");
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

Vec3 Tree::GetPos()
{
	return mParent.pos;
}

SphereCollider Tree::GetCollider()
{
	return mCollider;
}
