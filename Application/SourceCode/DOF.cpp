#include "DOF.h"
using namespace ConstantBufferData;

DOF::DOF() :
	focusWidth(0.05f), focusDepth(0.99f),
	dof_(std::make_unique<PostEffect>())
{
	tex_ = TextureManager::GetRenderTexture("DOF");
	tex_->useDepth = true;

	dof_->pos = GetWindowHalfSize();
	dof_->AddRenderTexture(tex_);
	dof_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("DOF"));
	dof_->AddMaterial(ConstantBuffer<CDOF>{});
}

void DOF::CreateGraphicsPipeline()
{
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};
	depthStencilDesc.DepthEnable = false; // 深度テストを行う

	std::string path = "Application/Shader/";

	// 被写界深度用
	ShaderObjectManager::Create("DOF");
	ShaderObjectManager::GetShaderObject("DOF")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("DOF")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("DOF")->CompileVertexShader(path + "DOFVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("DOF")->CompilePixelShader(path + "DOFPS.hlsl", "main");

	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("DOF"),
		RenderBase::GetInstance()->GetRenderTextureRootSignature()->GetRootSignature(),
		CullMode::None,
		depthStencilDesc,
		TopologyType::Triangle,
		1,
		"DOF");
}

void DOF::Update()
{
	CDOF dofData;

	dofData = { focusWidth,focusDepth };
	dof_->SetTransferBuffer(2, dofData);

	dof_->Update();
}

void DOF::DrawPostEffect()
{
	dof_->SetDrawCommands(2, 2);
	dof_->Draw();
}

void DOF::PrevSceneDraw()
{
	tex_->PrevDrawScene();
}

void DOF::PostSceneDraw()
{
	tex_->PostDrawScene();
}
