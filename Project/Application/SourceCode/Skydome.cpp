#include "Skydome.h"

void Skydome::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// ShadowObj用
	ShaderObjectManager::Create("Skydome");
	ShaderObjectManager::GetShaderObject("Skydome")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Skydome")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Skydome")->CompileVertexShader(path + "SkydomeVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Skydome")->CompilePixelShader(path + "SkydomePS.hlsl", "main");

	// 3Dオブジェクト用
	GraphicsPipelineSetting setting =
		GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Skydome");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.constantBufferViewNum = 3;
	GraphicsPipelineManager::Create(setting, "Skydome");
}

Skydome::Skydome() :
	mSkydome(std::make_unique<Object3D>()),
	mPostEffect(std::make_unique<PostEffect>())
{
	mSkydome->SetModel(ModelManager::GetModel("Skydome"));
	mSkydome->color = Color(0xc4c178);
	mSkydome->isLighting = false;

	for (uint32_t i = 0; i < 50; i++)
	{
		mClouds.emplace_back(std::move(std::make_unique<Cloud>()));

		const float range = 500.f;
		const Vec3 randomPos =
		{
			Random::RangeF(-range, range),
			150.f,
			Random::RangeF(-range, range),
		};
		mClouds.back()->SetPos(randomPos);
	}

	mRenderTexture = TextureManager::GetRenderTexture("Skydome");
	mRenderTexture->useDepth = false;
	mPostEffect->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Skydome"));
	mPostEffect->AddRenderTexture(mRenderTexture);
	mPostEffect->AddMaterial(ConstantBuffer<ConstantBufferData::CVignette>{});
	mPostEffect->pos = GetWindowHalfSize();

	isVignette = true;
	mVignetteData.range = Vec2(0.2f, 1.7f);
	mVignetteData.color = Color::black;
}

void Skydome::Init()
{
	mSkydome->scale = 800.f;
}

void Skydome::Update()
{
	mSkydome->pos.z = 200.f;
	mSkydome->Update();
	for (uint32_t i = 0; i < mClouds.size(); i++)
	{
		mClouds[i]->Update();
	}
	mPostEffect->SetTransferBuffer(2, mVignetteData);
	mPostEffect->Update();
}

void Skydome::RenderTextureSetting()
{
	mRenderTexture->PrevDrawScene();
	mSkydome->Draw();
	for (uint32_t i = 0; i < mClouds.size(); i++)
	{
		mClouds[i]->DrawModel();
	}
	mRenderTexture->PostDrawScene();
}

void Skydome::Draw()
{
	if (isVignette == false)
	{
		mSkydome->Draw();
		for (uint32_t i = 0; i < mClouds.size(); i++)
		{
			mClouds[i]->DrawModel();
		}
	}
	else
	{
		mPostEffect->SetDrawCommands(2, 2);
		mPostEffect->Draw();
	}
}

void Skydome::DrawDebugGui()
{
	Gui::BeginWindow("Skydome");
	Gui::DrawColorEdit("Skydome Color", mSkydome->color);
	Gui::DrawCheckBox("Use Vignette", &isVignette);
	Gui::DrawSlider2("Vignette Range", mVignetteData.range, 0.01f);
	Gui::DrawColorEdit("Vignette Color", mVignetteData.color);
	Gui::EndWindow();
}
