#include "RadialBlur.h"
using namespace ConstantBufferData;

RadialBlur::RadialBlur() :
	postEffect_(std::make_unique<PostEffect>())
{
	tex_ = TextureManager::GetRenderTexture("RadialBlur");

	postEffect_->pos = GetWindowHalfSize();
	postEffect_->AddRenderTexture(tex_);
	postEffect_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("RadialBlur"));
	postEffect_->AddMaterial(ConstantBuffer<CRadialBlur>{});
}

void RadialBlur::CreateGraphicsPipeline()
{
	//D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};
	//depthStencilDesc.DepthEnable = false; // 深度テストを行う

	//std::string path = "Application/Shader/";

	//// 高輝度抽出用
	//ShaderObjectManager::Create("RadialBlur");
	//ShaderObjectManager::GetShaderObject("RadialBlur")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	//ShaderObjectManager::GetShaderObject("RadialBlur")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	//ShaderObjectManager::GetShaderObject("RadialBlur")->CompileVertexShader(path + "RadialBlurVS.hlsl", "main");
	//ShaderObjectManager::GetShaderObject("RadialBlur")->CompilePixelShader(path + "RadialBlurPS.hlsl", "main");

	//GraphicsPipelineManager::Create(
	//	ShaderObjectManager::GetShaderObject("RadialBlur"),
	//	RenderBase::GetInstance()->GetRenderTextureRootSignature()->GetRootSignature(),
	//	CullMode::None,
	//	depthStencilDesc,
	//	TopologyType::Triangle,
	//	1,
	//	"RadialBlur");
}

void RadialBlur::Update()
{
	postEffect_->SetTransferBuffer(2, data_);
	postEffect_->Update();
}

void RadialBlur::DrawPostEffect()
{
	postEffect_->SetDrawCommands(2, 2);
	postEffect_->Draw();
}

void RadialBlur::PrevSceneDraw()
{
	tex_->PrevDrawScene();
}

void RadialBlur::PostSceneDraw()
{
	tex_->PostDrawScene();
}

void RadialBlur::DrawDebugGui()
{
	GuiManager::DrawSlider1("Strength", data_.strength, 0.01f);
	data_.strength = Max<float>(data_.strength, 0.f);

	GuiManager::DrawSlider1("LoopNum", data_.loopNum, 1.f);
	data_.loopNum = Clamp<float>(data_.loopNum, 1, 16);
}
