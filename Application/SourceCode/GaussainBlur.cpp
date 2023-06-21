#include "GaussainBlur.h"

GaussainBlur::GaussainBlur() :
	postEffect_(std::make_unique<PostEffect>())
{
	tex_ = TextureManager::GetRenderTexture("GaussainBlur");

	postEffect_->pos = GetWindowHalfSize();
	postEffect_->AddRenderTexture(tex_);
	postEffect_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("GaussainBlur"));
}

void GaussainBlur::CreateGraphicsPipeline()
{
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};
	depthStencilDesc.DepthEnable = false; // 深度テストを行う

	std::string path = "Application/Shader/";

	// 高輝度抽出用
	ShaderObjectManager::Create("GaussainBlur");
	ShaderObjectManager::GetShaderObject("GaussainBlur")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("GaussainBlur")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("GaussainBlur")->CompileVertexShader(path + "GaussainBlurVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("GaussainBlur")->CompilePixelShader(path + "GaussainBlurPS.hlsl", "main");

	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("GaussainBlur"),
		RenderBase::GetInstance()->GetRenderTextureRootSignature()->GetRootSignature(),
		CullMode::None,
		depthStencilDesc,
		TopologyType::Triangle,
		1,
		"GaussainBlur");
}

void GaussainBlur::Update()
{
	postEffect_->Update();
}

void GaussainBlur::DrawPostEffect()
{
	postEffect_->Draw();
}

void GaussainBlur::PrevSceneDraw()
{
	tex_->PrevDrawScene();
}

void GaussainBlur::PostSceneDraw()
{
	tex_->PostDrawScene();
}
