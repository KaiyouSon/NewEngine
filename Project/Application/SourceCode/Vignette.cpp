#include "Vignette.h"
using namespace ConstantBufferData;

Vignette::Vignette() :
	postEffect_(std::make_unique<PostEffect>())
{
	tex_ = TextureManager::GetRenderTexture("Vignette");

	postEffect_->pos = GetWindowHalfSize();
	postEffect_->AddRenderTexture(tex_);
	postEffect_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Vignette"));
	postEffect_->AddMaterial(ConstantBuffer<CVignette>{});

	vignetteData.range = Vec2(0.6f, 1.3f);
}

void Vignette::CreateGraphicsPipeline()
{
	//D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};
	//depthStencilDesc.DepthEnable = false; // 深度テストを行う

	//std::string path = "Application/Shader/";

	//ShaderObjectManager::Create("Vignette");
	//ShaderObjectManager::GetShaderObject("Vignette")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	//ShaderObjectManager::GetShaderObject("Vignette")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	//ShaderObjectManager::GetShaderObject("Vignette")->CompileVertexShader(path + "VignetteVS.hlsl", "main");
	//ShaderObjectManager::GetShaderObject("Vignette")->CompilePixelShader(path + "VignettePS.hlsl", "main");

	//GraphicsPipelineManager::Create(
	//	ShaderObjectManager::GetShaderObject("Vignette"),
	//	RenderBase::GetInstance()->GetRenderTextureRootSignature()->GetRootSignature(),
	//	CullMode::None,
	//	depthStencilDesc,
	//	TopologyType::Triangle,
	//	1,
	//	"Vignette");
}

void Vignette::Update()
{
	postEffect_->SetTransferBuffer(2, vignetteData);
	postEffect_->Update();
}

void Vignette::DrawPostEffect()
{
	postEffect_->SetDrawCommands(2, 2);
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

void Vignette::DrawDebugGui()
{
	GuiManager::DrawColorEdit("Vignette Color", vignetteData.color);
	vignetteData.color = vignetteData.color.To01();
	GuiManager::DrawSlider2("Vignette Range", vignetteData.range, 0.01f);
}
