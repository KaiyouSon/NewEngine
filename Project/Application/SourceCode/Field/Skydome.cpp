#include "Skydome.h"

void Skydome::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// 繝薙ロ繝・ヨ逕ｨ
	ShaderCompilerManager::Create("Skydome");
	ShaderCompilerManager::GetShaderCompiler("Skydome")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderCompilerManager::GetShaderCompiler("Skydome")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderCompilerManager::GetShaderCompiler("Skydome")->CompileVertexShader(path + "SkydomeVS.hlsl", "main");
	ShaderCompilerManager::GetShaderCompiler("Skydome")->CompilePixelShader(path + "SkydomePS.hlsl", "main");

	// 3D繧ｪ繝悶ず繧ｧ繧ｯ繝育畑
	GraphicsPipelineSetting setting =
		PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Skydome");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	PipelineManager::CreateGraphicsPipeline(setting, "Skydome");
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
	mPostEffect->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Skydome"));
	mPostEffect->AddRenderTexture(mRenderTexture);
	mPostEffect->AddMaterial<ConstantBuffer<ConstantBufferData::CVignette>>();
	mPostEffect->pos = GetWindowHalfSize();

	isVignette = true;
	mVignetteData.range = Vec2(0.2f, 1.7f);
	mVignetteData.color = Color::black;

	test = false;
}

void Skydome::Init()
{
	mSkydome->scale = 800.f;
}

void Skydome::Update()
{
	if (Key::GetKeyDown(DIK_Z))
	{
		test = false;
	}
	if (Key::GetKeyDown(DIK_C))
	{
		test = true;
	}

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
		//mPostEffect->SetDrawCommands(2, 2);
		mPostEffect->Draw();
	}
}

void Skydome::DrawDebugGui()
{
	if (test == true)
	{
		Gui::BeginWindow("Skydome");
		Gui::DrawColorEdit("Skydome Color", mSkydome->color);
		Gui::DrawCheckBox("Use Vignette", &isVignette);
		Gui::DrawSlider2("Vignette Range", mVignetteData.range, 0.01f);
		Gui::DrawColorEdit("Vignette Color", mVignetteData.color);
		Gui::EndWindow();
	}
}

