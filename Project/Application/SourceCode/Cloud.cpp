#include "Cloud.h"

void Cloud::CreateGraphicsPipeline()
{
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};
	depthStencilDesc.DepthEnable = true; // 深度テストを行う
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	// 書き込み許可
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 小さいほうを採用

	std::string path = "Application/Shader/";

	// 高輝度抽出用
	ShaderObjectManager::Create("Cloud");
	ShaderObjectManager::GetShaderObject("Cloud")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Cloud")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Cloud")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Cloud")->CompileVertexShader(path + "CloudVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Cloud")->CompilePixelShader(path + "CloudPS.hlsl", "main");

	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("Cloud"),
		RenderBase::GetInstance()->GetObject3DRootSignature()->GetRootSignature(),
		CullMode::Back,
		depthStencilDesc,
		TopologyType::Triangle,
		1,
		"Cloud");
}

Cloud::Cloud() :
	cloud_(std::make_unique<Object3D>())
{
	cloud_->SetModel(ModelManager::GetModel("Cloud"));
	cloud_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Cloud"));
	cloud_->color = Color(0xc4c178);

	Init();
}

void Cloud::Init()
{
	cloud_->pos.y = 180.f;
	cloud_->scale = 100.f;
	cloud_->rot.x = Radian(180);
}

void Cloud::Update()
{
	cloud_->offset.x += 0.0005f;

	cloud_->Update();
}

void Cloud::DrawModel()
{
	cloud_->Draw();
}

void Cloud::SetPos(const Vec3 pos)
{
	cloud_->pos = pos;
}
