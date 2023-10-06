#include "Bloom.h"

void Bloom::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// 鬮倩ｼ晏ｺｦ謚ｽ蜃ｺ逕ｨ
	ShaderCompilerManager::Create("HighLumi");
	ShaderCompilerManager::GetShaderCompiler("HighLumi")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderCompilerManager::GetShaderCompiler("HighLumi")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderCompilerManager::GetShaderCompiler("HighLumi")->CompileVertexShader(path + "HighLumiVS.hlsl", "main");
	ShaderCompilerManager::GetShaderCompiler("HighLumi")->CompilePixelShader(path + "HighLumiPS.hlsl", "main");

	GraphicsPipelineSetting setting = PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("HighLumi");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "HighLumi");

	// 繧ｬ繧ｦ繧ｷ繧｢繝ｳ繝悶Λ繝ｼ逕ｨ
	ShaderCompilerManager::Create("GaussianBlur");
	ShaderCompilerManager::GetShaderCompiler("GaussianBlur")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderCompilerManager::GetShaderCompiler("GaussianBlur")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderCompilerManager::GetShaderCompiler("GaussianBlur")->CompileVertexShader(path + "GaussianBlurVS.hlsl", "main");
	ShaderCompilerManager::GetShaderCompiler("GaussianBlur")->CompilePixelShader(path + "GaussianBlurPS.hlsl", "main");

	setting = PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("GaussianBlur");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "GaussianBlur");

	// 蜷域・逕ｨ
	ShaderCompilerManager::Create("Composite");
	ShaderCompilerManager::GetShaderCompiler("Composite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderCompilerManager::GetShaderCompiler("Composite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderCompilerManager::GetShaderCompiler("Composite")->CompileVertexShader(path + "CompositeVS.hlsl", "main");
	ShaderCompilerManager::GetShaderCompiler("Composite")->CompilePixelShader(path + "CompositePS.hlsl", "main");

	setting = PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Composite");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Composite");
}

Bloom::Bloom()
{
	// 繝・け繧ｹ繝√Ε繝ｼ縺ｮ繧ｻ繝・ヨ
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

	// 繝代う繝励Λ繧､繝ｳ險ｭ螳・
	mPasses[(uint32_t)PassType::HighLumi]->
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("HighLumi"));
	mPasses[(uint32_t)PassType::GaussianBlur]->
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GaussianBlur"));

	// 蜷域・逕ｨ繝代せ
	mCompositePass = std::make_unique<PostEffect>();
	mCompositePass->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Composite"));
	mCompositePass->AddRenderTexture(mTexs[(uint32_t)PassType::Bloom]);
	mCompositePass->AddRenderTexture(mTexs[(uint32_t)PassType::Target]);
	mCompositePass->pos = GetWindowHalfSize();
}

void Bloom::Update()
{
	for (uint32_t i = 0; i < mPasses.size(); i++)
	{
		mPasses[i]->Update();
	}

	mCompositePass->Update();
}

void Bloom::DrawPostEffect()
{
	mCompositePass->Draw();
}

void Bloom::DrawPass(const PassType passType)
{
	mPasses[(uint32_t)passType]->Draw();
}

void Bloom::PrevSceneDraw(const PassType passType)
{
	mTexs[(uint32_t)passType]->PrevDrawScene();
}

void Bloom::PostSceneDraw(const PassType passType)
{
	mTexs[(uint32_t)passType]->PostDrawScene();
}

