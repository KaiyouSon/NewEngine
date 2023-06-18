#include "Bloom.h"
#include "RenderBase.h"

Bloom::Bloom() :
	highLumi_(std::make_unique<PostEffect>()),
	blur_(std::make_unique<PostEffect>()),
	bloom_(std::make_unique<PostEffect>())
{
	texs_[0] = TextureManager::GetRenderTexture("HighLumi");
	texs_[1] = TextureManager::GetRenderTexture("GaussainBlur");
	texs_[2] = TextureManager::GetRenderTexture("Bloom");

	auto test = GraphicsPipelineManager::GetGraphicsPipeline("HighLumi");

	highLumi_->pos = GetWindowHalfSize();
	highLumi_->AddRenderTexture(texs_[0]);
	highLumi_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("HighLumi"));

	blur_->pos = GetWindowHalfSize();
	blur_->AddRenderTexture(texs_[1]);
	blur_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("GaussainBlur"));

	bloom_->pos = GetWindowHalfSize();
	bloom_->AddRenderTexture(texs_[0]);
	bloom_->AddRenderTexture(texs_[2]);
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
