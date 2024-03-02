#include "CreateManager.h"
#include "ShaderManager.h"

/// ------------------------------------------------------------- ///
/// --- シェダーコンパイラーの生成 ------------------------------ ///
/// ------------------------------------------------------------- ///
void CreateManager::CreateShaderCompiler()
{
	// 天球用
	ShaderManager::CompileAllType(AppShaderDirectory + "Skydome", "Skydome");

	// 草用
	ShaderManager::CompileAllType(AppShaderDirectory + "Grass", "Grass");

	// 雲用
	ShaderManager::CompileAllType(AppShaderDirectory + "Cloud", "Cloud");

	// 木の枝用
	ShaderManager::CompileAllType(AppShaderDirectory + "Branch", "Branch");

	// リスポーン地点用（下の波紋用）
	ShaderManager::CompileAllType(AppShaderDirectory + "RespawnPoint/Ripple", "Ripple");

	// リスポーン地点用（浮いてる菱形用）
	ShaderManager::CompileAllType(AppShaderDirectory + "RespawnPoint/Rhombus", "Rhombus");

	// 軌跡用
	ShaderManager::CompileAllType(AppShaderDirectory + "Trajectory", "Trajectory");

	// リスポーンエフェクト用
	ShaderManager::CompileAllType(AppShaderDirectory + "RespawnPointEffect", "RespawnPointEffect");

	// ポストプロセス
	{
		// ビネット用
		ShaderManager::CompileAllType(AppShaderDirectory + "PostEffect/Vignette");

		// 高輝度箇所抽出用（RenderTexture）
		ShaderManager::CompileAllType(AppShaderDirectory + "PostEffect/HighLumi");

		// ガウシアンブラー用（RenderTexture）
		ShaderManager::CompileAllType(AppShaderDirectory + "PostEffect/GaussianBlur");

		// 合成用（RenderTexture）
		ShaderManager::CompileAllType(AppShaderDirectory + "PostEffect/Composite");

		// レンズフレア用
		ShaderManager::CompileAllType(AppShaderDirectory + "PostEffect/LensFlare");

		// ラジアルブラー用（RenderTexture）
		ShaderManager::CompileAllType(AppShaderDirectory + "PostEffect/RadialBlur");

		// トーンマッピング用（RenderTexture）
		ShaderManager::CompileAllType(AppShaderDirectory + "PostEffect/ToneMapping");

		// リスポーン時の遷移用
		ShaderManager::CompileAllType(AppShaderDirectory + "Transition/RespawnTransition");

		// ShadowObj用
		ShaderManager::CompileAllType(AppShaderDirectory + "ShadowObj");

		// ShadowMap用
		ShaderManager::CompileAllType(AppShaderDirectory + "ShadowMap");
	}

	// GPUパーティクル関連
	{
		// タイトルのロゴ爆散用
		ShaderManager::CompileAllType(AppShaderDirectory + "Effect/LogoExplosionEffect");

		// 誘導エフェクト用
		ShaderManager::CompileAllType(AppShaderDirectory + "Effect/LeadEffect");

		// プレイヤー回復エフェクト用
		ShaderManager::CompileAllType(AppShaderDirectory + "Effect/PlayerRecoveryEffect");

		// 空中のエフェクト用
		ShaderManager::CompileAllType(AppShaderDirectory + "Effect/AirEffect");

		// ボスの攻撃軌跡のエフェクト用
		ShaderManager::CompileAllType(AppShaderDirectory + "Effect/BossAttackTrajectoryEffect");

		// 攻撃の爆発のエフェクト
		ShaderManager::CompileAllType(AppShaderDirectory + "Effect/AttackExplosionEffect");

		// 煙のエフェクト
		ShaderManager::CompileAllType(AppShaderDirectory + "Effect/SmokeEffect");

		// ポンデリング/太陽用
		ShaderManager::CompileAllType(AppShaderDirectory + "PonDeRing");
	}


	//// ワールドのボリュメトリックフォグ
	//ShaderCompilerSetting setting;
	//setting = ShaderCompilerSetting();
	//setting.mInputLayoutSettings.resize(2);
	//setting.mInputLayoutSettings[0] = InputLayoutSetting("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	//setting.mInputLayoutSettings[1] = InputLayoutSetting("TEXCOORD", DXGI_FORMAT_R32G32B32_FLOAT);
	//setting.vsFilePath = AppShaderDirectory + "WorldVolumetricFog/WorldVolumetricFogVS.hlsl";
	//setting.psFilePath = AppShaderDirectory + "WorldVolumetricFog/WorldVolumetricFogPS.hlsl";
	//ShaderCompilerManager::Create(setting, "WorldVolumetricFog");
}

/// ------------------------------------------------------------- ///
/// --- Graphicsパイプライン生成 -------------------------------- ///
/// ------------------------------------------------------------- ///
void CreateManager::CreateGraphicsPipeline()
{
	GraphicsPipelineSetting setting;
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};

	setting = PipelineManager::GetGraphicsPipelineSetting("Object3D");
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "Skydome");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "Skydome");
	setting.inputLayout = ShaderManager::GetInputLayout("Skydome");
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Skydome");

	// 草用
	setting = PipelineManager::GetGraphicsPipeline("Emitter")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "Grass");
	setting.gs = ShaderManager::GetShader(ShaderType::Geometry, "Grass");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "Grass");
	setting.inputLayout = ShaderManager::GetInputLayout("Grass");
	setting.rootSignatureSetting.maxCbvRootParameter = 5;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Grass");

	// 雲用
	depthStencilDesc = D3D12_DEPTH_STENCIL_DESC();
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "Cloud");
	setting.gs = ShaderManager::GetShader(ShaderType::Geometry, "Cloud");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "Cloud");
	setting.inputLayout = ShaderManager::GetInputLayout("Cloud");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc;
	PipelineManager::CreateGraphicsPipeline(setting, "Cloud");

	// ビネット用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "Vignette");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "Vignette");
	setting.inputLayout = ShaderManager::GetInputLayout("Vignette");
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Vignette");

	// 木の枝用
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "Branch");
	setting.gs = ShaderManager::GetShader(ShaderType::Geometry, "Branch");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "Branch");
	setting.inputLayout = ShaderManager::GetInputLayout("Branch");
	setting.cullMode = CullMode::None;
	PipelineManager::CreateGraphicsPipeline(setting, "Branch");

	// リスポーン地点用（下の波紋用）
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "Ripple");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "Ripple");
	setting.inputLayout = ShaderManager::GetInputLayout("Ripple");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	PipelineManager::CreateGraphicsPipeline(setting, "Ripple");

	// リスポーン地点用（浮いてる菱形用）
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "Rhombus");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "Rhombus");
	setting.inputLayout = ShaderManager::GetInputLayout("Rhombus");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleList;
	PipelineManager::CreateGraphicsPipeline(setting, "Rhombus");

	// 高輝度箇所抽出用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "HighLumi");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "HighLumi");
	setting.inputLayout = ShaderManager::GetInputLayout("HighLumi");
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	PipelineManager::CreateGraphicsPipeline(setting, "HighLumi");

	// ガウシアンブラー用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "GaussianBlur");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "GaussianBlur");
	setting.inputLayout = ShaderManager::GetInputLayout("GaussianBlur");
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "GaussianBlur");

	// 合成用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "Composite");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "Composite");
	setting.inputLayout = ShaderManager::GetInputLayout("Composite");
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Composite");

	// リスポーン時の遷移用
	setting = PipelineManager::GetGraphicsPipeline("Sprite")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "RespawnTransition");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "RespawnTransition");
	setting.inputLayout = ShaderManager::GetInputLayout("RespawnTransition");
	setting.rootSignatureSetting.maxCbvRootParameter = 4;
	PipelineManager::CreateGraphicsPipeline(setting, "RespawnTransition");

	// ShadowObj用
	setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "ShadowObj");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "ShadowObj");
	setting.inputLayout = ShaderManager::GetInputLayout("ShadowObj");
	setting.cullMode = CullMode::None;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "ShadowObj");

	// ShadowMap用
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "ShadowMap");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "ShadowMap");
	setting.inputLayout = ShaderManager::GetInputLayout("ShadowMap");
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "ShadowMap");

	// リスポーンエフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "RespawnPointEffect");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "RespawnPointEffect");
	setting.inputLayout = nullptr;
	PipelineManager::CreateGraphicsPipeline(setting, "RespawnPointEffect");

	// タイトルのロゴ爆散用
	setting = PipelineManager::GetGraphicsPipeline("ParticleMesh")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "LogoExplosionEffect");
	setting.gs = ShaderManager::GetShader(ShaderType::Geometry, "ParticleMesh");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "ParticleMesh");
	setting.inputLayout = nullptr;
	PipelineManager::CreateGraphicsPipeline(setting, "LogoExplosionEffect");

	// 誘導エフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "LeadEffect");
	setting.gs = ShaderManager::GetShader(ShaderType::Geometry, "GPUEmitter");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "GPUEmitter");
	setting.inputLayout = nullptr;
	PipelineManager::CreateGraphicsPipeline(setting, "LeadEffect");

	// プレイヤー回復エフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "PlayerRecoveryEffect");
	setting.gs = ShaderManager::GetShader(ShaderType::Geometry, "GPUEmitter");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "GPUEmitter");
	setting.inputLayout = nullptr;
	PipelineManager::CreateGraphicsPipeline(setting, "PlayerRecoveryEffect");

	// 空中のエフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "AirEffect");
	setting.gs = ShaderManager::GetShader(ShaderType::Geometry, "GPUEmitter");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "GPUEmitter");
	setting.inputLayout = nullptr;
	PipelineManager::CreateGraphicsPipeline(setting, "AirEffect");

	// ボスの攻撃軌跡のエフェクト用
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "BossAttackTrajectoryEffect");
	setting.gs = ShaderManager::GetShader(ShaderType::Geometry, "GPUEmitter");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "GPUEmitter");
	setting.inputLayout = nullptr;
	PipelineManager::CreateGraphicsPipeline(setting, "BossAttackTrajectoryEffect");

	// 攻撃の爆発のエフェクト
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "AttackExplosionEffect");
	setting.gs = ShaderManager::GetShader(ShaderType::Geometry, "GPUEmitter");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "GPUEmitter");
	setting.inputLayout = nullptr;
	PipelineManager::CreateGraphicsPipeline(setting, "AttackExplosionEffect");

	// 煙のエフェクト
	setting = PipelineManager::GetGraphicsPipeline("GPUEmitter")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "SmokeEffect");
	setting.gs = ShaderManager::GetShader(ShaderType::Geometry, "GPUEmitter");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "GPUEmitter");
	setting.inputLayout = nullptr;
	PipelineManager::CreateGraphicsPipeline(setting, "SmokeEffect");

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
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "Trajectory");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "Trajectory");
	setting.inputLayout = ShaderManager::GetInputLayout("Trajectory");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleStrip;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	setting.rootSignatureSetting.maxUavDescritorRange = 0;
	PipelineManager::CreateGraphicsPipeline(setting, "Trajectory");

	// ポンデリング/太陽用
	setting = PipelineManager::GetGraphicsPipeline("ParticleObject")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "PonDeRing");
	setting.gs = ShaderManager::GetShader(ShaderType::Geometry, "ParticleObject");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "ParticleObject");
	PipelineManager::CreateGraphicsPipeline(setting, "PonDeRing");



	//// 攻撃の爆発のエフェクト
	//depthStencilDesc = D3D12_DEPTH_STENCIL_DESC();
	//depthStencilDesc.DepthEnable = false;
	//depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	//depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	//setting = PipelineManager::GetGraphicsPipeline("VolumetricFog")->GetSetting();
	//setting.shaderCompilerTag = "WorldVolumetricFog";
	//setting.depthStencilDesc = depthStencilDesc;
	//PipelineManager::CreateGraphicsPipeline(setting, "WorldVolumetricFog");

	// ラジアルブラー用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "RadialBlur");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "RadialBlur");
	setting.inputLayout = ShaderManager::GetInputLayout("RadialBlur");
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "RadialBlur");

	// トーンマッピング用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "ToneMapping");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "ToneMapping");
	setting.inputLayout = ShaderManager::GetInputLayout("ToneMapping");
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "ToneMapping");

	// レンズフレア用（PostEffect）
	setting = PipelineManager::GetGraphicsPipeline("PostEffect")->GetSetting();
	setting.vs = ShaderManager::GetShader(ShaderType::Vertex, "LensFlare");
	setting.ps = ShaderManager::GetShader(ShaderType::Pixel, "LensFlare");
	setting.inputLayout = ShaderManager::GetInputLayout("LensFlare");
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
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "RespawnPointEffect");
	setting.rootSignatureSetting.maxCbvRootParameter = 0;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "RespawnPointEffect");

	// タイトルのロゴ爆散用（初期化）
	setting = PipelineManager::GetComputePipeline("ParticleMesh")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "LogoExplosionEffectInit");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	PipelineManager::CreateComputePipeline(setting, "LogoExplosionEffectInit");

	// タイトルのロゴ爆散用（更新）
	setting = PipelineManager::GetComputePipeline("ParticleMesh")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "LogoExplosionEffectUpdate");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	PipelineManager::CreateComputePipeline(setting, "LogoExplosionEffectUpdate");

	// 誘導エフェクト用（初期化）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "LeadEffectInit");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "LeadEffectInit");

	// 誘導エフェクト用（更新）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "LeadEffectUpdate");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "LeadEffectUpdate");

	// プレイヤー回復エフェクト用（初期化）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "PlayerRecoveryEffectInit");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 1;
	PipelineManager::CreateComputePipeline(setting, "PlayerRecoveryEffectInit");

	// プレイヤー回復エフェクト用（更新）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "PlayerRecoveryEffectUpdate");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 1;
	PipelineManager::CreateComputePipeline(setting, "PlayerRecoveryEffectUpdate");

	// 空中のエフェクト用（初期化）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "AirEffectInit");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "AirEffectInit");

	// 空中のエフェクト用（更新）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "AirEffectUpdate");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "AirEffectUpdate");

	// ボスの攻撃軌跡のエフェクト用（初期化用）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "BossAttackTrajectoryEffectInit");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "BossAttackTrajectoryEffectInit");

	// ボスの攻撃軌跡のエフェクト用（更新用）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "BossAttackTrajectoryEffectUpdate");
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 2;
	PipelineManager::CreateComputePipeline(setting, "BossAttackTrajectoryEffectUpdate");

	// ポンデリング/太陽用（初期化用）
	setting = PipelineManager::GetComputePipeline("ParticleObject")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "PonDeRingInit");
	setting.rootSignatureSetting.maxCbvRootParameter = 1;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	setting.rootSignatureSetting.maxUavDescritorRange = 1;
	PipelineManager::CreateComputePipeline(setting, "PonDeRingInit");

	// ポンデリング/太陽用（更新用）
	setting = PipelineManager::GetComputePipeline("ParticleObject")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "PonDeRingUpdate");
	setting.rootSignatureSetting.maxCbvRootParameter = 1;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	setting.rootSignatureSetting.maxUavDescritorRange = 1;
	PipelineManager::CreateComputePipeline(setting, "PonDeRingUpdate");

	// 攻撃の爆発のエフェクト（初期化用）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "AttackExplosionEffectInit");
	setting.rootSignatureSetting.maxCbvRootParameter = 1;
	PipelineManager::CreateComputePipeline(setting, "AttackExplosionEffectInit");

	// 攻撃の爆発のエフェクト（更新用）
	setting = PipelineManager::GetComputePipeline("GPUEmitter")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "AttackExplosionEffectUpdate");
	setting.rootSignatureSetting.maxCbvRootParameter = 1;
	PipelineManager::CreateComputePipeline(setting, "AttackExplosionEffectUpdate");

	// 煙のエフェクト用（初期化用）
	setting = PipelineManager::GetComputePipeline("BossAttackTrajectoryEffectInit")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "SmokeEffectInit");
	PipelineManager::CreateComputePipeline(setting, "SmokeEffectInit");

	// 煙のエフェクト用（更新用）
	setting = PipelineManager::GetComputePipeline("BossAttackTrajectoryEffectUpdate")->GetSetting();
	setting.cs = ShaderManager::GetShader(ShaderType::Compute, "SmokeEffectUpdate");
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
