#include "CreateManager.h"

// シェダーコンパイラーの生成
void CreateManager::CreateShaderCompiler()
{
	std::string path1 = "NewEngine/Shader/";
	std::string path2 = "Application/Shader/";

	ShaderCompilerSetting setting;

	// 草用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT, 1);
	setting.vsFilePath = path2 + "GrassVS.hlsl";
	setting.gsFilePath = path2 + "GrassGS.hlsl";
	setting.psFilePath = path2 + "GrassPS.hlsl";
	ShaderCompilerManager::Create(setting, "Grass");

	// 雲用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "CloudVS.hlsl";
	setting.psFilePath = path2 + "CloudPS.hlsl";
	ShaderCompilerManager::Create(setting, "Cloud");

	// 天球用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "SkydomeVS.hlsl";
	setting.psFilePath = path2 + "SkydomePS.hlsl";
	ShaderCompilerManager::Create(setting, "Skydome");

	// 木の枝用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "BranchVS.hlsl";
	setting.psFilePath = path2 + "BranchPS.hlsl";
	ShaderCompilerManager::Create(setting, "Branch");

	// リスポーン地点用（下の波紋用）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "RippleVS.hlsl";
	setting.psFilePath = path2 + "RipplePS.hlsl";
	ShaderCompilerManager::Create(setting, "Ripple");

	// リスポーン地点用（浮いてる菱形用）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "RhombusVS.hlsl";
	setting.psFilePath = path2 + "RhombusPS.hlsl";
	ShaderCompilerManager::Create(setting, "Rhombus");

	// 高輝度箇所抽出用（RenderTexture）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "HighLumiVS.hlsl";
	setting.psFilePath = path2 + "HighLumiPS.hlsl";
	ShaderCompilerManager::Create(setting, "HighLumi");

	// ガウシアンブラー用（RenderTexture）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "GaussianBlurVS.hlsl";
	setting.psFilePath = path2 + "GaussianBlurPS.hlsl";
	ShaderCompilerManager::Create(setting, "GaussianBlur");

	// 合成用（RenderTexture）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "CompositeVS.hlsl";
	setting.psFilePath = path2 + "CompositePS.hlsl";
	ShaderCompilerManager::Create(setting, "Composite");

	// リスポーン時の遷移用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "RespawnTransitionVS.hlsl";
	setting.psFilePath = path2 + "RespawnTransitionPS.hlsl";
	ShaderCompilerManager::Create(setting, "RespawnTransition");

	// ShadowObj用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "ShadowObjVS.hlsl";
	setting.psFilePath = path2 + "ShadowObjPS.hlsl";
	ShaderCompilerManager::Create(setting, "ShadowObj");

	// ShadowMap用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "ShadowMapVS.hlsl";
	setting.psFilePath = path2 + "ShadowMapPS.hlsl";
	ShaderCompilerManager::Create(setting, "ShadowMap");

	// リスポーンエフェクト用
	setting = ShaderCompilerSetting();
	setting.csFilePath = path2 + "RespawnPointEffectCS.hlsl";
	setting.vsFilePath = path2 + "RespawnPointEffectVS.hlsl";
	setting.gsFilePath = path1 + "EmitterGS.hlsl";
	setting.psFilePath = path1 + "EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "RespawnPointEffect");
}

// パイプライン生成
void CreateManager::CreateGraphicsPipeline()
{
	GraphicsPipelineSetting setting;
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};

	// 草用
	setting = PipelineManager::GetGraphicsPipeline("Emitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Grass");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Grass");

	// 雲用
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Cloud");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc;
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Cloud");

	// 天球用（RenderTexture）
	setting = PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Skydome");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	PipelineManager::CreateGraphicsPipeline(setting, "Skydome");

	// 木の枝用
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Branch");
	setting.cullMode = CullMode::None;
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Branch");

	// リスポーン地点用（下の波紋用）
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Ripple");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Ripple");

	// リスポーン地点用（浮いてる菱形用）
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Rhombus");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleList;
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Rhombus");

	// 高輝度箇所抽出用（RenderTexture）
	setting = PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("HighLumi");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "HighLumi");

	// ガウシアンブラー用（RenderTexture）
	setting = PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("GaussianBlur");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "GaussianBlur");

	// 合成用（RenderTexture）
	setting = PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Composite");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Composite");

	// リスポーン時の遷移用
	setting = PipelineManager::GetGraphicsPipeline("Sprite")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("RespawnTransition");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 4;
	PipelineManager::CreateGraphicsPipeline(setting, "RespawnTransition");

	// ShadowObj用
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("ShadowObj");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "ShadowObj");

	// ShadowMap用
	setting = PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("ShadowMap");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	PipelineManager::CreateGraphicsPipeline(setting, "ShadowMap");

	// 深度値のみ書き込み用
	{
		// 3Dオブジェクト
		setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
		setting.renderTargetBlendMask = GraphicsPipelineSetting::WriteNone;
		PipelineManager::CreateGraphicsPipeline(setting, "Object3DWriteNone");

		// 草
		setting = PipelineManager::GetGraphicsPipeline("Grass")->GetSetting();
		setting.renderTargetBlendMask = GraphicsPipelineSetting::WriteNone;
		PipelineManager::CreateGraphicsPipeline(setting, "GrassWriteNone");
	}

	// リスポーンエフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("RespawnPointEffect");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "RespawnPointEffect");
}

// Computeパイプラインの生成
void CreateManager::CreateComputePipeline()
{
	// リスポーンエフェクト用
	ComputePipelineSetting setting;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("RespawnPointEffect");
	setting.rootSignatureSetting.maxCbvRootParameter = 0;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "RespawnPointEffect");
}

void CreateManager::Create()
{
	// シェダーコンパイラーの生成
	CreateShaderCompiler();

	// Graphicsパイプライン生成
	CreateGraphicsPipeline();

	// Computeパイプライン生成
	CreateComputePipeline();
}
