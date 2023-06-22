#include "Vignette.h"

Vignette::Vignette() :
	postEffect_(std::make_unique<PostEffect>())
{
	tex_ = TextureManager::GetRenderTexture("Vignette");

	postEffect_->pos = GetWindowHalfSize();
	postEffect_->AddRenderTexture(tex_);
	postEffect_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Vignette"));
}

void Vignette::CreateGraphicsPipeline()
{
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};
	depthStencilDesc.DepthEnable = false; // 深度テストを行う

	std::string path = "Application/Shader/";

	ShaderObjectManager::Create("Vignette");
	ShaderObjectManager::GetShaderObject("Vignette")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Vignette")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Vignette")->CompileVertexShader(path + "VignetteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Vignette")->CompilePixelShader(path + "VignettePS.hlsl", "main");

	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("Vignette"),
		RenderBase::GetInstance()->GetRenderTextureRootSignature()->GetRootSignature(),
		CullMode::None,
		depthStencilDesc,
		TopologyType::Triangle,
		1,
		"Vignette");
}

void Vignette::Update()
{
	postEffect_->Update();
}

void Vignette::DrawPostEffect()
{
	postEffect_->Draw();
}

void Vignette::PrevSceneDraw()
{
	tex_->PrevDrawScene();
}

void Vignette::PostSceneDraw()
{
	tex_->PostDrawScene();
}
