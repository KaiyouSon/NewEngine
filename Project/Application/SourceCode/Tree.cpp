#include "Tree.h"

void Tree::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// ShadowObj用
	ShaderObjectManager::Create("Branch");
	ShaderObjectManager::GetShaderObject("Branch")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Branch")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Branch")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Branch")->CompileVertexShader(path + "BranchVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Branch")->CompilePixelShader(path + "BranchPS.hlsl", "main");

	// 3Dオブジェクト用
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
	mTree->SetisShadow(false, true);

	mBranch->SetModel(ModelManager::GetModel("Branch"));
	mBranch->SetisShadow(false, true);
	mBranch->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Branch"));
}

void Tree::Init()
{
	mPraent.pos = Vec3(-10, 0, 0);
	mPraent.scale = 3;
	mPraent.Update();

	mBranch->rot.y = Radian(286);
}

void Tree::Update()
{
	if (Key::GetKey(DIK_RIGHT))
	{
		mBranch->rot.y += Radian(1);
	}
	if (Key::GetKey(DIK_LEFT))
	{
		mBranch->rot.y -= Radian(1);
	}

	mTree->Update(&mPraent);
	mBranch->Update(&mPraent);
}

void Tree::DrawModel()
{
	mTree->Draw();
	mBranch->Draw();
}
