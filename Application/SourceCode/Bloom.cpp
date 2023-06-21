#include "Bloom.h"

Bloom::Bloom() :
	highLumi_(std::make_unique<PostEffect>()),
	blur_(std::make_unique<PostEffect>()),
	bloom_(std::make_unique<PostEffect>())
{
	texs_[0] = TextureManager::GetRenderTexture("HighLumi");
	texs_[1] = TextureManager::GetRenderTexture("BGaussainBlur");
	texs_[2] = TextureManager::GetRenderTexture("Bloom");

	highLumi_->pos = GetWindowHalfSize();
	highLumi_->AddRenderTexture(texs_[0]);
	highLumi_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("HighLumi"));

	blur_->pos = GetWindowHalfSize();
	blur_->AddRenderTexture(texs_[1]);
	blur_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("BGaussainBlur"));

	bloom_->pos = GetWindowHalfSize();
	bloom_->AddRenderTexture(texs_[2]);
	bloom_->AddRenderTexture(TextureManager::GetRenderTexture("BackGround"));
	bloom_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Bloom"));
}

void Bloom::CreateGraphicsPipeline()
{
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};
	depthStencilDesc.DepthEnable = false; // �[�x�e�X�g���s��

	std::string path = "Application/Shader/";

	// ���P�x���o�p
	ShaderObjectManager::Create("HighLumi");
	ShaderObjectManager::GetShaderObject("HighLumi")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("HighLumi")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("HighLumi")->CompileVertexShader(path + "HighLumiVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("HighLumi")->CompilePixelShader(path + "HighLumiPS.hlsl", "main");

	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("HighLumi"),
		RenderBase::GetInstance()->GetRenderTextureRootSignature()->GetRootSignature(),
		CullMode::None,
		depthStencilDesc,
		TopologyType::Triangle,
		1,
		"HighLumi");

	// �K�E�V�A���u���[�p
	ShaderObjectManager::Create("BGaussainBlur");
	ShaderObjectManager::GetShaderObject("BGaussainBlur")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("BGaussainBlur")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("BGaussainBlur")->CompileVertexShader(path + "BGaussainBlurVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("BGaussainBlur")->CompilePixelShader(path + "BGaussainBlurPS.hlsl", "main");

	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("BGaussainBlur"),
		RenderBase::GetInstance()->GetRenderTextureRootSignature()->GetRootSignature(),
		CullMode::None,
		depthStencilDesc,
		TopologyType::Triangle,
		1,
		"BGaussainBlur");

	// �u���[���p
	ShaderObjectManager::Create("Bloom");
	ShaderObjectManager::GetShaderObject("Bloom")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Bloom")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Bloom")->CompileVertexShader(path + "BloomVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Bloom")->CompilePixelShader(path + "BloomPS.hlsl", "main");

	GraphicsPipelineManager::Create(
		ShaderObjectManager::GetShaderObject("Bloom"),
		RenderBase::GetInstance()->GetRenderTextureRootSignature()->GetRootSignature(),
		CullMode::None,
		depthStencilDesc,
		TopologyType::Triangle,
		1,
		"Bloom");
}

void Bloom::Update()
{
	highLumi_->Update();
	blur_->Update();
	bloom_->Update();
}

void Bloom::DrawPostEffect(const uint32_t index)
{
	if (index == 0)
	{
		highLumi_->Draw();
	}
	else if (index == 1)
	{
		blur_->Draw();
	}
	else
	{
		bloom_->Draw();
	}
}

void Bloom::PrevSceneDraw(const uint32_t index)
{
	texs_[index]->PrevDrawScene();
}

void Bloom::PostSceneDraw(const uint32_t index)
{
	texs_[index]->PostDrawScene();
}
