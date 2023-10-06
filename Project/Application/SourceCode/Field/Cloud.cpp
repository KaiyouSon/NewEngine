#include "Cloud.h"

void Cloud::CreateGraphicsPipeline()
{
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};
	depthStencilDesc.DepthEnable = true; // 豺ｱ蠎ｦ繝・せ繝医ｒ陦後≧
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	// 譖ｸ縺崎ｾｼ縺ｿ險ｱ蜿ｯ
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 蟆上＆縺・⊇縺・ｒ謗｡逕ｨ

	std::string path = "Application/Shader/";


	GraphicsPipelineSetting setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Cloud");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc;
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Cloud");
}

Cloud::Cloud() :
	mCloud(std::make_unique<Object3D>())
{
	mCloud->SetModel(ModelManager::GetModel("Cloud"));
	mCloud->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Cloud"));
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
