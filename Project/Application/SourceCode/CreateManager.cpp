#include "CreateManager.h"

/// ------------------------------------------------------------- ///
/// --- シェダーコンパイラーの生成 ------------------------------ ///
/// ------------------------------------------------------------- ///
void CreateManager::CreateShaderCompiler()
{
	std::string path1 = "NewEngine/Shader/";

	ShaderCompilerSetting setting;

	// 天球用
	ShaderCompilerManager::Create(AppShaderDirectory + "Skydome", "Skydome");

	// 草用
	ShaderCompilerManager::Create(AppShaderDirectory + "Grass", "Grass");

	// ビネット用
	ShaderCompilerManager::Create(AppShaderDirectory + "PostEffect/Vignette", "Vignette");

	// 木の枝用
	ShaderCompilerManager::Create(AppShaderDirectory + "Branch", "Branch");

	// リスポーン地点用（下の波紋用）
	ShaderCompilerManager::Create(AppShaderDirectory + "RespawnPoint/Ripple", "Ripple");

	// リスポーン地点用（浮いてる菱形用）
	ShaderCompilerManager::Create(AppShaderDirectory + "RespawnPoint/Rhombus", "Rhombus");

	// 高輝度箇所抽出用（RenderTexture）
	ShaderCompilerManager::Create(AppShaderDirectory + "PostEffect/HighLumi", "HighLumi");

	// ガウシアンブラー用（RenderTexture）
	ShaderCompilerManager::Create(AppShaderDirectory + "PostEffect/GaussianBlur", "GaussianBlur");

	// 合成用（RenderTexture）
	ShaderCompilerManager::Create(AppShaderDirectory + "PostEffect/Composite", "Composite");

	// リスポーン時の遷移用
	ShaderCompilerManager::Create(AppShaderDirectory + "Transition/RespawnTransition", "RespawnTransition");

	// ShadowObj用
	ShaderCompilerManager::Create(AppShaderDirectory + "ShadowObj", "ShadowObj");

	// ShadowMap用
	ShaderCompilerManager::Create(AppShaderDirectory + "ShadowMap", "ShadowMap");

	// 軌跡用
	ShaderCompilerManager::Create(AppShaderDirectory + "Trajectory", "Trajectory");

	// レンズフレア用
	ShaderCompilerManager::Create(AppShaderDirectory + "PostEffect/LensFlare", "LensFlare");

	// 雲用
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(3);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[2] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	setting.vsFilePath = AppShaderDirectory + "Cloud/CloudVS.hlsl";
	setting.psFilePath = AppShaderDirectory + "Cloud/CloudPS.hlsl";
	ShaderCompilerManager::Create(setting, "Cloud");

	// リスポーンエフェクト用
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "RespawnPointEffect/RespawnPointEffectCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "RespawnPointEffect/RespawnPointEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "RespawnPointEffect");

	// タイトルのロゴ爆散用（初期化）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "Effect/LogoExplosionEffect/LogoExplosionEffectInitCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "Effect/LogoExplosionEffect/LogoExplosionEffectVS.hlsl";
	setting.gsFilePath = path1 + "ParticleMesh/ParticleMeshGS.hlsl";
	setting.psFilePath = path1 + "ParticleMesh/ParticleMeshPS.hlsl";
	ShaderCompilerManager::Create(setting, "LogoExplosionEffectInit");

	// タイトルのロゴ爆散用（更新）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "Effect/LogoExplosionEffect/LogoExplosionEffectUpdateCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "Effect/LogoExplosionEffect/LogoExplosionEffectVS.hlsl";
	setting.gsFilePath = path1 + "ParticleMesh/ParticleMeshGS.hlsl";
	setting.psFilePath = path1 + "ParticleMesh/ParticleMeshPS.hlsl";
	ShaderCompilerManager::Create(setting, "LogoExplosionEffectUpdate");

	// 誘導エフェクト用（初期化）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "Effect/LeadEffect/LeadEffectInitCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "Effect/LeadEffect/LeadEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "LeadEffectInit");

	// 誘導エフェクト用（更新）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "Effect/LeadEffect/LeadEffectUpdateCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "Effect/LeadEffect/LeadEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "LeadEffectUpdate");

	// プレイヤー回復エフェクト用（初期化）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "Effect/PlayerRecoveryEffect/PlayerRecoveryEffectInitCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "Effect/PlayerRecoveryEffect/PlayerRecoveryEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "PlayerRecoveryEffectInit");

	// プレイヤー回復エフェクト用（更新）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "Effect/PlayerRecoveryEffect/PlayerRecoveryEffectUpdateCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "Effect/PlayerRecoveryEffect/PlayerRecoveryEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "PlayerRecoveryEffectUpdate");

	// 空中のエフェクト用（初期化）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "Effect/AirEffect/AirEffectInitCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "Effect/AirEffect/AirEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "AirEffectInit");

	// 空中のエフェクト用（更新）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "Effect/AirEffect/AirEffectUpdateCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "Effect/AirEffect/AirEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "AirEffectUpdate");

	// ボスの攻撃軌跡のエフェクト用（初期化用）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "Effect/BossAttackTrajectoryEffect/BossAttackTrajectoryEffectInitCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "Effect/BossAttackTrajectoryEffect/BossAttackTrajectoryEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "BossAttackTrajectoryEffectInit");

	// ボスの攻撃軌跡のエフェクト用（更新用）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "Effect/BossAttackTrajectoryEffect/BossAttackTrajectoryEffectUpdateCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "Effect/BossAttackTrajectoryEffect/BossAttackTrajectoryEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "BossAttackTrajectoryEffectUpdate");

	// ポンデリング/太陽用（初期化用）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "PonDeRing/PonDeRingInitCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "PonDeRing/PonDeRingVS.hlsl";
	setting.gsFilePath = path1 + "ParticleObject/ParticleObjectGS.hlsl";
	setting.psFilePath = path1 + "ParticleObject/ParticleObjectPS.hlsl";
	ShaderCompilerManager::Create(setting, "PonDeRingInit");

	// ポンデリング/太陽用（更新用）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "PonDeRing/PonDeRingUpdateCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "PonDeRing/PonDeRingVS.hlsl";
	setting.gsFilePath = path1 + "ParticleObject/ParticleObjectGS.hlsl";
	setting.psFilePath = path1 + "ParticleObject/ParticleObjectPS.hlsl";
	ShaderCompilerManager::Create(setting, "PonDeRingUpdate");

	// 攻撃の爆発のエフェクト（初期化用）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "Effect/AttackExplosionEffect/AttackExplosionEffectInitCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "Effect/AttackExplosionEffect/AttackExplosionEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "AttackExplosionEffectInit");

	// 攻撃の爆発のエフェクト（更新用）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "Effect/AttackExplosionEffect/AttackExplosionEffectUpdateCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "Effect/AttackExplosionEffect/AttackExplosionEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "AttackExplosionEffectUpdate");

	// 煙のエフェクト（初期化用）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "Effect/SmokeEffect/SmokeEffectInitCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "Effect/SmokeEffect/SmokeEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "SmokeEffectInit");

	// 煙のエフェクト（更新用）
	setting = ShaderCompilerSetting();
	setting.csFilePath = AppShaderDirectory + "Effect/SmokeEffect/SmokeEffectUpdateCS.hlsl";
	setting.vsFilePath = AppShaderDirectory + "Effect/SmokeEffect/SmokeEffectVS.hlsl";
	setting.gsFilePath = path1 + "Emitter/EmitterGS.hlsl";
	setting.psFilePath = path1 + "Emitter/EmitterPS.hlsl";
	ShaderCompilerManager::Create(setting, "SmokeEffectUpdate");

	// ワールドのボリュメトリックフォグ
	setting = ShaderCompilerSetting();
	setting.mInputLayoutSettings.resize(2);
	setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32B32_FLOAT);
	setting.vsFilePath = AppShaderDirectory + "WorldVolumetricFog/WorldVolumetricFogVS.hlsl";
	setting.psFilePath = AppShaderDirectory + "WorldVolumetricFog/WorldVolumetricFogPS.hlsl";
	ShaderCompilerManager::Create(setting, "WorldVolumetricFog");

	// ラジアルブラー用（RenderTexture）
	ShaderCompilerManager::Create(AppShaderDirectory + "PostEffect/RadialBlur", "RadialBlur");

	// トーンマッピング用（RenderTexture）
	ShaderCompilerManager::Create(AppShaderDirectory + "PostEffect/ToneMapping", "ToneMapping");
}

/// ------------------------------------------------------------- ///
/// --- Graphicsパイプライン生成 -------------------------------- ///
/// ------------------------------------------------------------- ///
void CreateManager::CreateGraphicsPipeline()
{
	GraphicsPipelineSetting setting;
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};

	setting = GraphicsPipelineSetting();
	setting = PipelineManager::GetGraphicsPipelineSetting("Object3D");
	setting.shaderCompilerTag = "Skydome";
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Object3D");

	// 草用
	setting = PipelineManager::GetGraphicsPipeline("Emitter")->GetSetting();
	setting.shaderCompilerTag = "Grass";
	setting.rootSignatureSetting.maxCbvRootParameter = 5;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Grass");

	// 雲用
	depthStencilDesc = D3D12_DEPTH_STENCIL_DESC();
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderCompilerTag = "Cloud";
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc;
	PipelineManager::CreateGraphicsPipeline(setting, "Cloud");

	// ビネット用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.shaderCompilerTag = "Vignette";
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Vignette");

	// 木の枝用
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderCompilerTag = "Branch";
	setting.cullMode = CullMode::None;
	PipelineManager::CreateGraphicsPipeline(setting, "Branch");

	// リスポーン地点用（下の波紋用）
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderCompilerTag = "Ripple";
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	PipelineManager::CreateGraphicsPipeline(setting, "Ripple");

	// リスポーン地点用（浮いてる菱形用）
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderCompilerTag = "Rhombus";
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleList;
	PipelineManager::CreateGraphicsPipeline(setting, "Rhombus");

	// 高輝度箇所抽出用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.shaderCompilerTag = "HighLumi";
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	PipelineManager::CreateGraphicsPipeline(setting, "HighLumi");

	// ガウシアンブラー用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.shaderCompilerTag = "GaussianBlur";
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "GaussianBlur");

	// 合成用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.shaderCompilerTag = "Composite";
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Composite");

	// リスポーン時の遷移用
	setting = PipelineManager::GetGraphicsPipeline("Sprite")->GetSetting();
	setting.shaderCompilerTag = "RespawnTransition";
	setting.rootSignatureSetting.maxCbvRootParameter = 4;
	PipelineManager::CreateGraphicsPipeline(setting, "RespawnTransition");

	// ShadowObj用
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderCompilerTag = "ShadowObj";
	setting.cullMode = CullMode::None;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "ShadowObj");

	// ShadowMap用
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.shaderCompilerTag = "ShadowMap";
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "ShadowMap");

	// リスポーンエフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.shaderCompilerTag = "RespawnPointEffect";
	PipelineManager::CreateGraphicsPipeline(setting, "RespawnPointEffect");

	// タイトルのロゴ爆散用
	setting = PipelineManager::GetGraphicsPipeline("ParticleMesh")->GetSetting();
	setting.shaderCompilerTag = "LogoExplosionEffectInit";
	PipelineManager::CreateGraphicsPipeline(setting, "LogoExplosionEffect");

	// 誘導エフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.shaderCompilerTag = "LeadEffectInit";
	PipelineManager::CreateGraphicsPipeline(setting, "LeadEffect");

	// プレイヤー回復エフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.shaderCompilerTag = "PlayerRecoveryEffectInit";
	PipelineManager::CreateGraphicsPipeline(setting, "PlayerRecoveryEffect");

	// 空中のエフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.shaderCompilerTag = "AirEffectInit";
	PipelineManager::CreateGraphicsPipeline(setting, "AirEffect");

	// ボスの攻撃軌跡のエフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.shaderCompilerTag = "BossAttackTrajectoryEffectInit";
	PipelineManager::CreateGraphicsPipeline(setting, "BossAttackTrajectoryEffect");

	// 深度値のみ書き込み用
	{
		// 3Dオブジェクト
		setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
		setting.renderTargetBlendMask = GraphicsPipelineSetting::WriteNone;
		PipelineManager::CreateGraphicsPipeline2(setting, "Object3DWriteNone");

		// 草
		setting = PipelineManager::GetGraphicsPipeline("Grass")->GetSetting();
		setting.renderTargetBlendMask = GraphicsPipelineSetting::WriteNone;
		PipelineManager::CreateGraphicsPipeline(setting, "GrassWriteNone");

		// 木
		setting = PipelineManager::GetGraphicsPipeline("Branch")->GetSetting();
		setting.renderTargetBlendMask = GraphicsPipelineSetting::WriteNone;
		PipelineManager::CreateGraphicsPipeline(setting, "BranchWriteNone");

		// リスポーン地点用（下の波紋用）
		setting = PipelineManager::GetGraphicsPipeline("Ripple")->GetSetting();
		setting.renderTargetBlendMask = GraphicsPipelineSetting::WriteNone;
		PipelineManager::CreateGraphicsPipeline(setting, "RippleWriteNone");

		// リスポーン地点用（浮いてる菱形用）
		setting = PipelineManager::GetGraphicsPipeline("Rhombus")->GetSetting();
		setting.renderTargetBlendMask = GraphicsPipelineSetting::WriteNone;
		PipelineManager::CreateGraphicsPipeline(setting, "RhombusWriteNone");
	}

	// 軌跡用
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderCompilerTag = "Trajectory";
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleStrip;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	setting.rootSignatureSetting.maxUavDescritorRange = 0;
	PipelineManager::CreateGraphicsPipeline(setting, "Trajectory");

	// ポンデリング/太陽用
	setting = PipelineManager::GetGraphicsPipeline("ParticleObject")->GetSetting();
	setting.shaderCompilerTag = "PonDeRingInit";
	PipelineManager::CreateGraphicsPipeline(setting, "PonDeRing");

	// 攻撃の爆発のエフェクト
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.shaderCompilerTag = "AttackExplosionEffectInit";
	PipelineManager::CreateGraphicsPipeline(setting, "AttackExplosionEffect");

	// 攻撃の爆発のエフェクト
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.shaderCompilerTag = "SmokeEffectInit";
	PipelineManager::CreateGraphicsPipeline(setting, "SmokeEffect");

	// 攻撃の爆発のエフェクト
	depthStencilDesc = D3D12_DEPTH_STENCIL_DESC();
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	setting = PipelineManager::GetGraphicsPipeline("VolumetricFog")->GetSetting();
	setting.shaderCompilerTag = "WorldVolumetricFog";
	setting.depthStencilDesc = depthStencilDesc;
	PipelineManager::CreateGraphicsPipeline(setting, "WorldVolumetricFog");

	// ラジアルブラー用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.shaderCompilerTag = "RadialBlur";
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "RadialBlur");

	// トーンマッピング用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.shaderCompilerTag = "ToneMapping";
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "ToneMapping");

	// レンズフレア用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.shaderCompilerTag = "LensFlare";
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "LensFlare");
}

/// ------------------------------------------------------------- ///
/// --- Computeパイプラインの生成 ------------------------------- ///
/// ------------------------------------------------------------- ///
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
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "AirEffectInit");

	// 空中のエフェクト用（更新）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("AirEffectUpdate");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "AirEffectUpdate");

	// ボスの攻撃軌跡のエフェクト用（初期化用）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("BossAttackTrajectoryEffectInit");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "BossAttackTrajectoryEffectInit");

	// ボスの攻撃軌跡のエフェクト用（更新用）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("BossAttackTrajectoryEffectUpdate");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "BossAttackTrajectoryEffectUpdate");

	// ポンデリング/太陽用（初期化用）
	setting = PipelineManager::GetComputePipeline("ParticleObject")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("PonDeRingInit");
	setting.rootSignatureSetting.maxCbvRootParameter = 1;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	setting.rootSignatureSetting.maxUavDescritorRange = 1;
	PipelineManager::CreateComputePipeline(setting, "PonDeRingInit");

	// ポンデリング/太陽用（更新用）
	setting = PipelineManager::GetComputePipeline("ParticleObject")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("PonDeRingUpdate");
	setting.rootSignatureSetting.maxCbvRootParameter = 1;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	setting.rootSignatureSetting.maxUavDescritorRange = 1;
	PipelineManager::CreateComputePipeline(setting, "PonDeRingUpdate");

	// 攻撃の爆発のエフェクト（初期化用）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("AttackExplosionEffectInit");
	setting.rootSignatureSetting.maxCbvRootParameter = 1;
	PipelineManager::CreateComputePipeline(setting, "AttackExplosionEffectInit");

	// 攻撃の爆発のエフェクト（更新用）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("AttackExplosionEffectUpdate");
	setting.rootSignatureSetting.maxCbvRootParameter = 1;
	PipelineManager::CreateComputePipeline(setting, "AttackExplosionEffectUpdate");

	// ボスの攻撃軌跡のエフェクト用（初期化用）
	setting = PipelineManager::GetComputePipeline("BossAttackTrajectoryEffectInit")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("SmokeEffectInit");
	PipelineManager::CreateComputePipeline(setting, "SmokeEffectInit");

	// ボスの攻撃軌跡のエフェクト用（更新用）
	setting = PipelineManager::GetComputePipeline("BossAttackTrajectoryEffectUpdate")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("SmokeEffectUpdate");
	PipelineManager::CreateComputePipeline(setting, "SmokeEffectUpdate");
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
