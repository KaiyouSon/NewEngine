#include "Cloud.h"

void Cloud::CreateGraphicsPipeline()
{
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};
	depthStencilDesc.DepthEnable = true; // 深度テストを行う
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	// 書き込み許可
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 小さいほうを採用

	std::string path = "Application/Shader/";

	// 高輝度抽出用
	ShaderObjectManager::Create("Cloud");
	ShaderObjectManager::GetShaderObject("Cloud")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Cloud")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Cloud")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Cloud")->CompileVertexShader(path + "CloudVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Cloud")->CompilePixelShader(path + "CloudPS.hlsl", "main");

	GraphicsPipelineSetting setting;
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Cloud");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.constantBufferViewNum = 7;
	setting.rootSignatureSetting.descriptorRangeNum = 3;
	GraphicsPipelineManager::Create(setting, "Cloud");

}

Cloud::Cloud() :
	mCloud(std::make_unique<Object3D>())
{
	mCloud->SetModel(ModelManager::GetModel("Cloud"));
	mCloud->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Cloud"));
	mCloud->color = Color(0xc4c178);

	Init();
}

void Cloud::Init()
{
	mCloud->pos.y = 180.f;
	mCloud->scale = 100.f;
	mCloud->rot.x = Radian(180);
}

void Cloud::Update()
{
	mCloud->offset.x += 0.0005f;

	mCloud->Update();
}

void Cloud::DrawModel()
{
	mCloud->Draw();
}

void Cloud::SetPos(const Vec3 pos)
{
	mCloud->pos = pos;
}
