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
	dof_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("DOF"));
	dof_->AddMaterial(ConstantBuffer<CDOF>{});
}

void DOF::CreateGraphicsPipeline()
{
	//D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};
	//depthStencilDesc.DepthEnable = false; // 深度テストを行う

	//std::string path = "Application/Shader/";

	//// 被写界深度用
	//ShaderObjectManager::Create("DOF");
	//ShaderObjectManager::GetShaderObject("DOF")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	//ShaderObjectManager::GetShaderObject("DOF")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	//ShaderObjectManager::GetShaderObject("DOF")->CompileVertexShader(path + "DOFVS.hlsl", "main");
	//ShaderObjectManager::GetShaderObject("DOF")->CompilePixelShader(path + "DOFPS.hlsl", "main");

	//GraphicsPipelineManager::Create(
	//	ShaderObjectManager::GetShaderObject("DOF"),
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
	GuiManager::DrawSlider1("FocusDepth", dofData_.focusDepth, 0.0001f);
	GuiManager::DrawSlider1("MiddleDepth", dofData_.middleDepth, 0.0001f);

	bool flag = dofData_.isRGB;
	GuiManager::DrawCheckBox("isRGB", &flag);
	dofData_.isRGB = flag;
}
