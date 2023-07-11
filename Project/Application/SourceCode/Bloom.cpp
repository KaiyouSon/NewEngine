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
	bloom_->AddRenderTexture(TextureManager::GetRenderTexture("CurrentScene"));
	bloom_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Bloom"));
}

void Bloom::CreateGraphicsPipeline()
{
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};
	depthStencilDesc.DepthEnable = false; // 深度テストを行う

	std::string path = "Application/Shader/";

	// 高輝度抽出用
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

	// ガウシアンブラー用
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

	// ブルーム用
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

void Bloom::DrawPostEffect(const TexType texType)
{
	if (texType == TexType::HighLumi)
	{
		highLumi_->Draw();
	}
	else if (texType == TexType::Blur)
	{
		blur_->Draw();
	}
	else
	{
		bloom_->Draw();
	}
}

void Bloom::PrevSceneDraw(const TexType texType)
{
	texs_[(uint32_t)texType]->PrevDrawScene();
}

void Bloom::PostSceneDraw(const TexType texType)
{
	texs_[(uint32_t)texType]->PostDrawScene();
}