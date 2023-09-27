#include "Bloom.h"

void Bloom::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// ���P�x���o�p
	ShaderObjectManager::Create("HighLumi");
	ShaderObjectManager::GetShaderObject("HighLumi")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("HighLumi")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("HighLumi")->CompileVertexShader(path + "HighLumiVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("HighLumi")->CompilePixelShader(path + "HighLumiPS.hlsl", "main");

	GraphicsPipelineSetting setting = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("HighLumi");
	setting.rtvNum = 1;
	GraphicsPipelineManager::Create(setting, "HighLumi");

	// �K�E�V�A���u���[�p
	ShaderObjectManager::Create("GaussianBlur");
	ShaderObjectManager::GetShaderObject("GaussianBlur")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("GaussianBlur")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("GaussianBlur")->CompileVertexShader(path + "GaussianBlurVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("GaussianBlur")->CompilePixelShader(path + "GaussianBlurPS.hlsl", "main");

	setting = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("GaussianBlur");
	setting.rtvNum = 1;
	GraphicsPipelineManager::Create(setting, "GaussianBlur");

	// �����p
	ShaderObjectManager::Create("Composite");
	ShaderObjectManager::GetShaderObject("Composite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Composite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Composite")->CompileVertexShader(path + "CompositeVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Composite")->CompilePixelShader(path + "CompositePS.hlsl", "main");

	setting = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Composite");
	setting.rtvNum = 1;
	GraphicsPipelineManager::Create(setting, "Composite");
}

Bloom::Bloom()
{
	// �e�N�X�`���[�̃Z�b�g
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

	// �p�C�v���C���ݒ�
	mPasses[(uint32_t)PassType::HighLumi]->
		SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("HighLumi"));
	mPasses[(uint32_t)PassType::GaussianBlur]->
		SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("GaussianBlur"));

	// �����p�p�X
	mCompositePass = std::make_unique<PostEffect>();
	mCompositePass->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Composite"));
	mCompositePass->AddRenderTexture(mTexs[(uint32_t)PassType::Bloom]);
	mCompositePass->AddRenderTexture(mTexs[(uint32_t)PassType::Target]);
	mCompositePass->pos = GetWindowHalfSize();

	flag = true;
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