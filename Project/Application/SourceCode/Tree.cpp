#include "Tree.h"

void Tree::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// ShadowObj�p
	ShaderObjectManager::Create("Branch");
	ShaderObjectManager::GetShaderObject("Branch")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Branch")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Branch")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Branch")->CompileVertexShader(path + "BranchVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Branch")->CompilePixelShader(path + "BranchPS.hlsl", "main");

	// 3D�I�u�W�F�N�g�p
	GraphicsPipelineSetting setting =
		GraphicsPipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Branch");
	setting.cullMode = CullMode::None;
	setting.rtvNum = 1;
	GraphicsPipelineManager::Create(setting, "Branch");
}

Tree::Tree() :
	mTree(std::make_unique<Object3D>()),
	mBranch(std::make_unique<Object3D>())
{
	mTree->SetModel(ModelManager::GetModel("Tree"));
	mBranch->SetModel(ModelManager::GetModel("Branch"));
}

void Tree::Init()
{
	mTree->SetisShadow(false, true);
	mBranch->SetisShadow(false, true);
	mBranch->rot.y = Radian(286);
	mBranch->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Branch"));
}

void Tree::Update()
{
	mTree->Update(&mPraent);
	mBranch->Update(&mPraent);
}

void Tree::DrawModel()
{
	mTree->Draw();
	mBranch->Draw();
}

void Tree::SetParent(const Transform parent)
{
	mPraent = parent;
	mPraent.Update();
}
