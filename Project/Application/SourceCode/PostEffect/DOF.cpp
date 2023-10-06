#include "DOF.h"
using namespace ConstantBufferData;

DOF::DOF() :
	dofData_({ 0.99f,0.995f, false }),
	dof_(std::make_unique<PostEffect>())
{
	tex_ = TextureManager::GetRenderTexture("DOF");
	tex_->useDepth = true;

	dof_->pos = GetWindowHalfSize();
	dof_->AddRenderTexture(tex_);
	dof_->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("DOF"));
	dof_->AddMaterial<ConstantBuffer<CDOF>>();
}

void DOF::CreateGraphicsPipeline()
{
	//D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};
	//depthStencilDesc.DepthEnable = false; // 豺ｱ蠎ｦ繝・せ繝医ｒ陦後≧

	//std::string path = "Application/Shader/";

	//// 陲ｫ蜀咏阜豺ｱ蠎ｦ逕ｨ
	//ShaderCompilerManager::Create("DOF");
	//ShaderCompilerManager::GetShaderCompiler("DOF")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	//ShaderCompilerManager::GetShaderCompiler("DOF")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	//ShaderCompilerManager::GetShaderCompiler("DOF")->CompileVertexShader(path + "DOFVS.hlsl", "main");
	//ShaderCompilerManager::GetShaderCompiler("DOF")->CompilePixelShader(path + "DOFPS.hlsl", "main");

	//PipelineManager::CreateGraphicsPipeline(
	//	ShaderCompilerManager::GetShaderCompiler("DOF"),
	//	RenderBase::GetInstance()->GetRenderTextureRootSignature()->GetRootSignature(),
	//	CullMode::None,
	//	depthStencilDesc,
	//	TopologyType::Triangle,
	//	1,
	//	"DOF");
}

void DOF::Update()
{
	dof_->SetTransferBuffer(2, dofData_);
	dof_->Update();
}

void DOF::DrawPostEffect()
{
	dof_->SetDrawCommands(2, 2);
	dof_->Draw();
}

void DOF::PrevSceneDraw()
{
	tex_->PrevDrawScene();
}

void DOF::PostSceneDraw()
{
	tex_->PostDrawScene();
}

void DOF::DrawDebugGui()
{
	Gui::DrawSlider1("FocusDepth", dofData_.focusDepth, 0.0001f);
	Gui::DrawSlider1("MiddleDepth", dofData_.middleDepth, 0.0001f);

	bool flag = dofData_.isRGB;
	Gui::DrawCheckBox("isRGB", &flag);
	dofData_.isRGB = flag;
}

