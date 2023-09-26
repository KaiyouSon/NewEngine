#include "Bloom.h"

void Bloom::CreateGraphicsPipeline()
{
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc{};
	depthStencilDesc.DepthEnable = false; // 深度テストを行う

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc1{};
	depthStencilDesc1.DepthEnable = true; // 深度テストを行う
	depthStencilDesc1.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 書き込み許可
	depthStencilDesc1.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 小さいほうを採用

	std::string path = "Application/Shader/";

	// 高輝度抽出用
	ShaderObjectManager::Create("HighLumi");
	ShaderObjectManager::GetShaderObject("HighLumi")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("HighLumi")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("HighLumi")->CompileVertexShader(path + "HighLumiVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("HighLumi")->CompilePixelShader(path + "HighLumiPS.hlsl", "main");

	GraphicsPipelineSetting setting = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("HighLumi");
	setting.rtvNum = 1;
	setting.depthStencilDesc = depthStencilDesc1;
	GraphicsPipelineManager::Create(setting, "HighLumi");

	// ガウシアンブラー用
	ShaderObjectManager::Create("GaussianBlur");
	ShaderObjectManager::GetShaderObject("GaussianBlur")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("GaussianBlur")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("GaussianBlur")->CompileVertexShader(path + "GaussianBlurVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("GaussianBlur")->CompilePixelShader(path + "GaussianBlurPS.hlsl", "main");

	setting = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("GaussianBlur");
	setting.rtvNum = 1;
	GraphicsPipelineManager::Create(setting, "GaussianBlur");

	// 合成用
	ShaderObjectManager::Create("Composite");
	ShaderObjectManager::GetShaderObject("Composite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Composite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Composite")->CompileVertexShader(path + "CompositeVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Composite")->CompilePixelShader(path + "CompositePS.hlsl", "main");

	setting = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Composite");
	setting.rtvNum = 1;
	GraphicsPipelineManager::Create(setting, "Composite");

	//// ブルーム用
	//ShaderObjectManager::Create("Bloom");
	//ShaderObjectManager::GetShaderObject("Bloom")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	//ShaderObjectManager::GetShaderObject("Bloom")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	//ShaderObjectManager::GetShaderObject("Bloom")->CompileVertexShader(path + "BloomVS.hlsl", "main");
	//ShaderObjectManager::GetShaderObject("Bloom")->CompilePixelShader(path + "BloomPS.hlsl", "main");

	//GraphicsPipelineManager::Create(
	//	ShaderObjectManager::GetShaderObject("Bloom"),
	//	RenderBase::GetInstance()->GetRenderTextureRootSignature()->GetRootSignature(),
	//	CullMode::None,
	//	depthStencilDesc,
	//	TopologyType::Triangle,
	//	1,
	//	"Bloom");
}

Bloom::Bloom()
{
	// テクスチャーのセット
	mTexs[(uint32_t)PassType::HighLumi] = TextureManager::GetRenderTexture("HighLumi");
	mTexs[(uint32_t)PassType::GaussianBlur] = TextureManager::GetRenderTexture("GaussianBlur");
	mTexs[(uint32_t)PassType::Bloom] = TextureManager::GetRenderTexture("Bloom");
	mTexs[(uint32_t)PassType::Target] = TextureManager::GetRenderTexture("EffectBloom");

	// 
	for (uint32_t i = 0; i < mPasses.size(); i++)
	{
		mPasses[i] = std::make_unique<PostEffect>();
		mPasses[i]->pos = GetWindowHalfSize()/* * -1*/;
	}
	mPasses[(uint32_t)PassType::HighLumi]->AddRenderTexture(mTexs[(uint32_t)PassType::HighLumi]);
	mPasses[(uint32_t)PassType::GaussianBlur]->AddRenderTexture(mTexs[(uint32_t)PassType::GaussianBlur]);

	// パイプライン設定
	mPasses[(uint32_t)PassType::HighLumi]->
		SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("HighLumi"));
	mPasses[(uint32_t)PassType::GaussianBlur]->
		SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("GaussianBlur"));

	// 合成用パス
	mCompositePass = std::make_unique<PostEffect>();
	mCompositePass->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Composite"));
	mCompositePass->AddRenderTexture(mTexs[(uint32_t)PassType::Bloom]);
	mCompositePass->AddRenderTexture(mTexs[(uint32_t)PassType::Target]);
	mCompositePass->pos = GetWindowHalfSize();

	flag = false;

	//texs_[TexType::HighLumi] = TextureManager::GetRenderTexture("HighLumi");
	////texs_[1] = TextureManager::GetRenderTexture("BGaussianBlur");
	////texs_[2] = TextureManager::GetRenderTexture("Bloom");

	//highLumi_->pos = GetWindowHalfSize();
	//highLumi_->AddRenderTexture(texs_[0]);
	//highLumi_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("HighLumi"));

	//blur_->pos = GetWindowHalfSize();
	//blur_->AddRenderTexture(texs_[1]);
	//blur_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("BGaussianBlur"));

	//bloom_->pos = GetWindowHalfSize();
	//bloom_->AddRenderTexture(texs_[2]);
	//bloom_->AddRenderTexture(TextureManager::GetRenderTexture("CurrentScene"));
	//bloom_->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Bloom"));
}

void Bloom::Update()
{
	if (Key::GetKeyDown(DIK_Q))
	{
		flag = true;
	}
	if (Key::GetKeyDown(DIK_E))
	{
		flag = false;
	}

	for (uint32_t i = 0; i < mPasses.size(); i++)
	{
		mPasses[i]->Update();
	}

	mCompositePass->Update();
}

void Bloom::DrawPostEffect()
{
	if (flag == true)
	{
		mCompositePass->Draw();
	}
}

void Bloom::DrawPass(const PassType passType)
{
	if (flag == true)
	{
		mPasses[(uint32_t)passType]->Draw();
	}
}

void Bloom::PrevSceneDraw(const PassType passType)
{
	mTexs[(uint32_t)passType]->PrevDrawScene();
}

void Bloom::PostSceneDraw(const PassType passType)
{
	mTexs[(uint32_t)passType]->PostDrawScene();
}