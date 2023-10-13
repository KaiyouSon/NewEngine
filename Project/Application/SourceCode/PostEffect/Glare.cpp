#include "Glare.h"
using namespace ConstantBufferData;

Glare::Glare() :
	highLumi_(std::make_unique<PostEffect>()),
	dotFilter_(std::make_unique<PostEffect>()),
	blur45_(std::make_unique<PostEffect>()),
	blur135_(std::make_unique<PostEffect>()),
	glare_(std::make_unique<PostEffect>()),
	result_(std::make_unique<PostEffect>()),
	lineBlurData1_({ 45 }), lineBlurData2_({ 135 })
{
	texs_[0] = TextureManager::GetRenderTexture("HighLumi");
	texs_[1] = TextureManager::GetRenderTexture("DotFilter");
	texs_[2] = TextureManager::GetRenderTexture("GLineBlur");
	texs_[3] = TextureManager::GetRenderTexture("Glare1");
	texs_[4] = TextureManager::GetRenderTexture("Glare2");
	texs_[5] = TextureManager::GetRenderTexture("Glare3");

	highLumi_->pos = GetWindowHalfSize();
	highLumi_->AddRenderTexture(texs_[0]);
	highLumi_->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("HighLumi"));

	dotFilter_->pos = GetWindowHalfSize();
	dotFilter_->AddRenderTexture(texs_[1]);
	dotFilter_->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("DotFilter"));

	blur45_->pos = GetWindowHalfSize();
	blur45_->AddRenderTexture(texs_[2]);
	blur45_->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GLineBlur"));
	blur45_->AddMaterial<ConstantBuffer<CLineBlur>>();

	blur135_->pos = GetWindowHalfSize();
	blur135_->AddRenderTexture(texs_[2]);
	blur135_->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GLineBlur"));
	blur135_->AddMaterial<ConstantBuffer<CLineBlur>>();

	glare_->pos = GetWindowHalfSize();
	glare_->AddRenderTexture(texs_[3]);
	glare_->AddRenderTexture(texs_[4]);
	glare_->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Glare"));

	result_->pos = GetWindowHalfSize();
	result_->AddRenderTexture(texs_[5]);
	result_->AddRenderTexture(TextureManager::GetRenderTexture("BackGround"));
	result_->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Glare"));
}

void Glare::CreateGraphicsPipeline()
{
	//D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};
	//depthStencilDesc.DepthEnable = false; // 豺ｱ蠎ｦ繝・せ繝医ｒ陦後≧

	//std::string path = "Application/Shader/";

	//// 繝ｩ繧､繝ｳ繝悶Λ繝ｼ逕ｨ
	//ShaderCompilerManager::Create("GLineBlur");
	//ShaderCompilerManager::GetShaderCompiler("GLineBlur")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	//ShaderCompilerManager::GetShaderCompiler("GLineBlur")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	//ShaderCompilerManager::GetShaderCompiler("GLineBlur")->CompileVertexShader(path + "GLineBlurVS.hlsl", "main");
	//ShaderCompilerManager::GetShaderCompiler("GLineBlur")->CompilePixelShader(path + "GLineBlurPS.hlsl", "main");

	//PipelineManager::CreateGraphicsPipeline(
	//	ShaderCompilerManager::GetShaderCompiler("GLineBlur"),
	//	RenderBase::GetInstance()->GetRenderTextureRootSignature()->GetRootSignature(),
	//	CullMode::None,
	//	depthStencilDesc,
	//	TopologyType::Triangle,
	//	1,
	//	"GLineBlur");

	//// 繧ｰ繝ｬ繧｢逕ｨ
	//ShaderCompilerManager::Create("Glare");
	//ShaderCompilerManager::GetShaderCompiler("Glare")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	//ShaderCompilerManager::GetShaderCompiler("Glare")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	//ShaderCompilerManager::GetShaderCompiler("Glare")->CompileVertexShader(path + "GlareVS.hlsl", "main");
	//ShaderCompilerManager::GetShaderCompiler("Glare")->CompilePixelShader(path + "GlarePS.hlsl", "main");

	//PipelineManager::CreateGraphicsPipeline(
	//	ShaderCompilerManager::GetShaderCompiler("Glare"),
	//	RenderBase::GetInstance()->GetRenderTextureRootSignature()->GetRootSignature(),
	//	CullMode::None,
	//	depthStencilDesc,
	//	TopologyType::Triangle,
	//	1,
	//	"Glare");

	//// 繝峨ャ繝医ヵ繧｣繝ｫ繧ｿ繝ｼ逕ｨ
	//ShaderCompilerManager::Create("DotFilter");
	//ShaderCompilerManager::GetShaderCompiler("DotFilter")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	//ShaderCompilerManager::GetShaderCompiler("DotFilter")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	//ShaderCompilerManager::GetShaderCompiler("DotFilter")->CompileVertexShader(path + "DotFilterVS.hlsl", "main");
	//ShaderCompilerManager::GetShaderCompiler("DotFilter")->CompilePixelShader(path + "DotFilterPS.hlsl", "main");

	//PipelineManager::CreateGraphicsPipeline(
	//	ShaderCompilerManager::GetShaderCompiler("DotFilter"),
	//	RenderBase::GetInstance()->GetRenderTextureRootSignature()->GetRootSignature(),
	//	CullMode::None,
	//	depthStencilDesc,
	//	TopologyType::Triangle,
	//	1,
	//	"DotFilter");
}

void Glare::Update()
{
	blur45_->SetTransferBuffer(2, lineBlurData1_);
	blur135_->SetTransferBuffer(2, lineBlurData2_);

	highLumi_->Update();
	dotFilter_->Update();
	blur45_->Update();
	blur135_->Update();
	glare_->Update();
	result_->Update();
}

void Glare::DrawPostEffect(const uint32_t index)
{
	if (index == 0)
	{
		highLumi_->Draw();
	}
	else if (index == 1)
	{
		dotFilter_->Draw();
	}
	else if (index == 2)
	{
		blur45_->SetDrawCommands(2, 2);
		blur45_->Draw();
	}
	else if (index == 3)
	{
		blur135_->SetDrawCommands(2, 2);
		blur135_->Draw();
	}
	else if (index == 4)
	{
		glare_->Draw();
	}
	else
	{
		result_->Draw();
	}
}

void Glare::PrevSceneDraw(const uint32_t index)
{
	texs_[index]->PrevDrawScene();
}

void Glare::PostSceneDraw(const uint32_t index)
{
	texs_[index]->PostDrawScene();
}

void Glare::DrawDebugGui()
{
	Gui::DrawSlider1("LineBlur1 Angle", lineBlurData1_.angle, 1.f);
	Gui::DrawSlider1("LineBlur2 Angle", lineBlurData2_.angle, 1.f);
}

