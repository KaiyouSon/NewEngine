#include "GaussianBlur.h"

GaussianBlur::GaussianBlur() :
	postEffect_(std::make_unique<PostEffect>())
{
	tex_ = TextureManager::GetRenderTexture("GaussianBlur");

	postEffect_->pos = GetWindowHalfSize();
	postEffect_->AddRenderTexture(tex_);
	postEffect_->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GaussianBlur"));
}

void GaussianBlur::CreateGraphicsPipeline()
{
	//D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};
	//depthStencilDesc.DepthEnable = false; // 深度テストを行う

	//std::string path = "Application/Shader/";

	//ShaderObjectManager::Create("GaussianBlur");
	//ShaderObjectManager::GetShaderObject("GaussianBlur")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	//ShaderObjectManager::GetShaderObject("GaussianBlur")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	//ShaderObjectManager::GetShaderObject("GaussianBlur")->CompileVertexShader(path + "GaussianBlurVS.hlsl", "main");
	//ShaderObjectManager::GetShaderObject("GaussianBlur")->CompilePixelShader(path + "GaussianBlurPS.hlsl", "main");

	//PipelineManager::CreateGraphicsPipeline(
	//	ShaderObjectManager::GetShaderObject("GaussianBlur"),
	//	RenderBase::GetInstance()->GetRenderTextureRootSignature()->GetRootSignature(),
	//	CullMode::None,
	//	depthStencilDesc,
	//	TopologyType::Triangle,
	//	1,
	//	"GaussianBlur");
}

void GaussianBlur::Update()
{
	postEffect_->Update();
}

void GaussianBlur::DrawPostEffect()
{
	postEffect_->Draw();
}

void GaussianBlur::PrevSceneDraw()
{
	tex_->PrevDrawScene();
}

void GaussianBlur::PostSceneDraw()
{
	tex_->PostDrawScene();
}
