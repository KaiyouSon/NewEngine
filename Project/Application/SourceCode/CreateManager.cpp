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
	setting.vsFilePath = path2 + "Grass/GrassVS.hlsl";
	setting.gsFilePath = path2 + "Grass/GrassGS.hlsl";
	setting.psFilePath = path2 + "Grass/GrassPS.hlsl";
	ShaderCompilerManager::Create(setting, "Grass");

	// 雲用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "Cloud/CloudVS.hlsl";
	setting.psFilePath = path2 + "Cloud/CloudPS.hlsl";
	ShaderCompilerManager::Create(setting, "Cloud");

	// ビネット用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "Vignette/VignetteVS.hlsl";
	setting.psFilePath = path2 + "Vignette/VignettePS.hlsl";
	ShaderCompilerManager::Create(setting, "Vignette");

	// 木の枝用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "Branch/BranchVS.hlsl";
	setting.psFilePath = path2 + "Branch/BranchPS.hlsl";
	ShaderCompilerManager::Create(setting, "Branch");

	// リスポーン地点用（下の波紋用）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "Ripple/RippleVS.hlsl";
	setting.psFilePath = path2 + "Ripple/RipplePS.hlsl";
	ShaderCompilerManager::Create(setting, "Ripple");

	// リスポーン地点用（浮いてる菱形用）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "Rhombus/RhombusVS.hlsl";
	setting.psFilePath = path2 + "Rhombus/RhombusPS.hlsl";
	ShaderCompilerManager::Create(setting, "Rhombus");

	// 高輝度箇所抽出用（RenderTexture）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "HighLumi/HighLumiVS.hlsl";
	setting.psFilePath = path2 + "HighLumi/HighLumiPS.hlsl";
	ShaderCompilerManager::Create(setting, "HighLumi");

	// ガウシアンブラー用（RenderTexture）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "GaussianBlur/GaussianBlurVS.hlsl";
	setting.psFilePath = path2 + "GaussianBlur/GaussianBlurPS.hlsl";
	ShaderCompilerManager::Create(setting, "GaussianBlur");

	// 合成用（RenderTexture）
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "Composite/CompositeVS.hlsl";
	setting.psFilePath = path2 + "Composite/CompositePS.hlsl";
	ShaderCompilerManager::Create(setting, "Composite");

	// リスポーン時の遷移用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "RespawnTransition/RespawnTransitionVS.hlsl";
	setting.psFilePath = path2 + "RespawnTransition/RespawnTransitionPS.hlsl";
	ShaderCompilerManager::Create(setting, "RespawnTransition");

	// ShadowObj用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "ShadowObj/ShadowObjVS.hlsl";
	setting.psFilePath = path2 + "ShadowObj/ShadowObjPS.hlsl";
	ShaderCompilerManager::Create(setting, "ShadowObj");

	// ShadowMap用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = path2 + "ShadowMap/ShadowMapVS.hlsl";
	setting.psFilePath = path2 + "ShadowMap/ShadowMapPS.hlsl";
	ShaderCompilerManager::Create(setting, "ShadowMap");

	// リスポーンエフェクト用
	setting = ShaderCompilerSetting();
	setting.csFilePath = path2 + "RespawnPointEffect/RespawnPointEffectCS.hlsl";
	setting.vsFilePath = path2 + "RespawnPointEffect/RespawnPointEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "RespawnPointEffect");

	// ボリューメトリックフォグ用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.vsFilePath = path2 + "VolumetricFog/VolumetricFogVS.hlsl";
	setting.psFilePath = path2 + "VolumetricFog/VolumetricFogPS.hlsl";
	ShaderCompilerManager::Create(setting, "VolumetricFog");

	// タイトルのロゴ爆散用（初期化）
	setting = ShaderCompilerSetting();
	setting.csFilePath = path2 + "Effect/LogoExplosionEffect/LogoExplosionEffectInitCS.hlsl";
	setting.vsFilePath = path2 + "Effect/LogoExplosionEffect/LogoExplosionEffectVS.hlsl";
	setting.gsFilePath = path1 + "ParticleMesh/ParticleMeshGS.hlsl";
	setting.psFilePath = path1 + "ParticleMesh/ParticleMeshPS.hlsl";
	ShaderCompilerManager::Create(setting, "LogoExplosionEffectInit");

	// タイトルのロゴ爆散用（更新）
	setting = ShaderCompilerSetting();
	setting.csFilePath = path2 + "Effect/LogoExplosionEffect/LogoExplosionEffectUpdateCS.hlsl";
	setting.vsFilePath = path2 + "Effect/LogoExplosionEffect/LogoExplosionEffectVS.hlsl";
	setting.gsFilePath = path1 + "ParticleMesh/ParticleMeshGS.hlsl";
	setting.psFilePath = path1 + "ParticleMesh/ParticleMeshPS.hlsl";
	ShaderCompilerManager::Create(setting, "LogoExplosionEffectUpdate");

	// 誘導エフェクト用（初期化）
	setting = ShaderCompilerSetting();
	setting.csFilePath = path2 + "Effect/LeadEffect/LeadEffectInitCS.hlsl";
	setting.vsFilePath = path2 + "Effect/LeadEffect/LeadEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "LeadEffectInit");

	// 誘導エフェクト用（更新）
	setting = ShaderCompilerSetting();
	setting.csFilePath = path2 + "Effect/LeadEffect/LeadEffectUpdateCS.hlsl";
	setting.vsFilePath = path2 + "Effect/LeadEffect/LeadEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "LeadEffectUpdate");

	// プレイヤー回復エフェクト用（初期化）
	setting = ShaderCompilerSetting();
	setting.csFilePath = path2 + "Effect/PlayerRecoveryEffect/PlayerRecoveryEffectInitCS.hlsl";
	setting.vsFilePath = path2 + "Effect/PlayerRecoveryEffect/PlayerRecoveryEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "PlayerRecoveryEffectInit");

	// プレイヤー回復エフェクト用（更新）
	setting = ShaderCompilerSetting();
	setting.csFilePath = path2 + "Effect/PlayerRecoveryEffect/PlayerRecoveryEffectUpdateCS.hlsl";
	setting.vsFilePath = path2 + "Effect/PlayerRecoveryEffect/PlayerRecoveryEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "PlayerRecoveryEffectUpdate");

	// 空中のエフェクト用（初期化）
	setting = ShaderCompilerSetting();
	setting.csFilePath = path2 + "Effect/AirEffect/AirEffectInitCS.hlsl";
	setting.vsFilePath = path2 + "Effect/AirEffect/AirEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "AirEffectInit");

	// 空中のエフェクト用（更新）
	setting = ShaderCompilerSetting();
	setting.csFilePath = path2 + "Effect/AirEffect/AirEffectUpdateCS.hlsl";
	setting.vsFilePath = path2 + "Effect/AirEffect/AirEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "AirEffectUpdate");
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
	depthStencilDesc = D3D12_DEPTH_STENCIL_DESC();
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

	// ビネット用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Vignette");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	PipelineManager::CreateGraphicsPipeline(setting, "Vignette");

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

	// 高輝度箇所抽出用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("HighLumi");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	PipelineManager::CreateGraphicsPipeline(setting, "HighLumi");

	// ガウシアンブラー用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("GaussianBlur");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "GaussianBlur");

	// 合成用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Composite");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
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
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("ShadowMap");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "ShadowMap");

	// リスポーンエフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("RespawnPointEffect");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "RespawnPointEffect");

	// ボリューメトリックフォグ用
	depthStencilDesc = D3D12_DEPTH_STENCIL_DESC();
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	setting = GraphicsPipelineSetting();
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("VolumetricFog");
	setting.cullMode = CullMode::Front;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 5;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "VolumetricFog");

	// タイトルのロゴ爆散用
	setting = PipelineManager::GetGraphicsPipeline("ParticleMesh")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("LogoExplosionEffectInit");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "LogoExplosionEffect");

	// 誘導エフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("LeadEffectInit");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "LeadEffect");

	// プレイヤー回復エフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("PlayerRecoveryEffectInit");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "PlayerRecoveryEffect");

	// 空中のエフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("AirEffectInit");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "AirEffect");

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

		// 木
		setting = PipelineManager::GetGraphicsPipeline("Branch")->GetSetting();
		setting.renderTargetBlendMask = GraphicsPipelineSetting::WriteNone;
		PipelineManager::CreateGraphicsPipeline(setting, "BranchWriteNone");

		// ボリューメトリックフォグ
		setting = PipelineManager::GetGraphicsPipeline("VolumetricFog")->GetSetting();
		setting.renderTargetBlendMask = GraphicsPipelineSetting::WriteNone;
		PipelineManager::CreateGraphicsPipeline(setting, "VolumetricFogWriteNone");
	}
}

// Computeパイプラインの生成
void CreateManager::CreateComputePipeline()
{
	// リスポーンエフェクト用
	ComputePipelineSetting setting;
	setting = ComputePipelineSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("RespawnPointEffect");
	setting.rootSignatureSetting.maxCbvRootParameter = 0;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "RespawnPointEffect");

	// タイトルのロゴ爆散用（初期化）
	setting = PipelineManager::GetComputePipeline("ParticleMesh")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("LogoExplosionEffectInit");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	PipelineManager::CreateComputePipeline(setting, "LogoExplosionEffectInit");

	// タイトルのロゴ爆散用（更新）
	setting = PipelineManager::GetComputePipeline("ParticleMesh")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("LogoExplosionEffectUpdate");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	PipelineManager::CreateComputePipeline(setting, "LogoExplosionEffectUpdate");

	// 誘導エフェクト用（初期化）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("LeadEffectInit");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "LeadEffectInit");

	// 誘導エフェクト用（更新）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("LeadEffectUpdate");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "LeadEffectUpdate");

	// プレイヤー回復エフェクト用（初期化）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("PlayerRecoveryEffectInit");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 1;
	PipelineManager::CreateComputePipeline(setting, "PlayerRecoveryEffectInit");

	// プレイヤー回復エフェクト用（更新）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("PlayerRecoveryEffectUpdate");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 1;
	PipelineManager::CreateComputePipeline(setting, "PlayerRecoveryEffectUpdate");

	// 空中のエフェクト用（初期化）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("AirEffectInit");
	setting.rootSignatureSetting.maxCbvRootParameter = 1;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "AirEffectInit");

	// 空中のエフェクト用（更新）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("AirEffectUpdate");
	setting.rootSignatureSetting.maxCbvRootParameter = 1;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "AirEffectUpdate");
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
