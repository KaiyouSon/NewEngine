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
	//depthStencilDesc.DepthEnable = false; // 豺ｱ蠎ｦ繝・せ繝医ｒ陦後≧

	//std::string path = "Application/Shader/";

	//ShaderCompilerManager::Create("GaussianBlur");
	//ShaderCompilerManager::GetShaderCompiler("GaussianBlur")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	//ShaderCompilerManager::GetShaderCompiler("GaussianBlur")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	//ShaderCompilerManager::GetShaderCompiler("GaussianBlur")->CompileVertexShader(path + "GaussianBlurVS.hlsl", "main");
	//ShaderCompilerManager::GetShaderCompiler("GaussianBlur")->CompilePixelShader(path + "GaussianBlurPS.hlsl", "main");

	//PipelineManager::CreateGraphicsPipeline(
	//	ShaderCompilerManager::GetShaderCompiler("GaussianBlur"),
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

