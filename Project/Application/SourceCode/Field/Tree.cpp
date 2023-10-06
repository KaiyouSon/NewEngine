#include "Tree.h"

void Tree::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// 3D繧ｪ繝悶ず繧ｧ繧ｯ繝育畑
	GraphicsPipelineSetting setting =
		PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Branch");
	setting.cullMode = CullMode::None;
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Branch");
}

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
