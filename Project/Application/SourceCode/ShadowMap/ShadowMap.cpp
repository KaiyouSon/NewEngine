#include "ShadowMap.h"
using namespace ConstantBufferData;

void ShadowMap::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// ShadowObj用
	ShaderCompilerManager::Create("ShadowMap");
	ShaderCompilerManager::GetShaderCompiler("ShadowMap")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderCompilerManager::GetShaderCompiler("ShadowMap")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderCompilerManager::GetShaderCompiler("ShadowMap")->CompileVertexShader(path + "ShadowMapVS.hlsl", "main");
	ShaderCompilerManager::GetShaderCompiler("ShadowMap")->CompilePixelShader(path + "ShadowMapPS.hlsl", "main");

	// 3Dオブジェクト用
	GraphicsPipelineSetting setting =
		PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("ShadowMap");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	PipelineManager::CreateGraphicsPipeline(setting, "ShadowMap");

	// ShadowObj用
	ShaderCompilerManager::Create("ShadowMapBlur");
	ShaderCompilerManager::GetShaderCompiler("ShadowMapBlur")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderCompilerManager::GetShaderCompiler("ShadowMapBlur")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderCompilerManager::GetShaderCompiler("ShadowMapBlur")->CompileVertexShader(path + "GaussianBlurVS.hlsl", "main");
	ShaderCompilerManager::GetShaderCompiler("ShadowMapBlur")->CompilePixelShader(path + "GaussianBlurPS.hlsl", "main");

	// 3Dオブジェクト用
	setting =
		PipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("ShadowMapBlur");
	setting.rtvNum = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "ShadowMapBlur");

}

ShadowMap::ShadowMap() :
	mShadowMap(std::make_unique<PostEffect>()),
	mShadowMapRT(TextureManager::GetRenderTexture("ShadowMap")),
	mBlur(std::make_unique<PostEffect>()),
	mBlurRT(TextureManager::GetRenderTexture("ShadowMapBlur"))
{
	mShadowMapRT->useDepth = true;

	mShadowMap->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ShadowMap"));
	mShadowMap->AddRenderTexture(mShadowMapRT);
	//mShadowMap->scale = 1.f / 32.f;
	//mShadowMap->pos = GetWindowHalfSize() / 2;
	mShadowMap->scale = 1.f / 32.f;
	mShadowMap->pos = GetWindowHalfSize();
	mShadowMap->AddMaterial<ConstantBuffer<CTransformShadowObj>>();

	mBlur->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ShadowMapBlur"));
	mBlur->AddRenderTexture(mBlurRT);
	//mBlur->scale = 1.f / 4.f;
	mBlur->scale = 1.f / 4.f;
	mBlur->pos = GetWindowHalfSize() / 2;

	mIndex = 0;

	mLightCamera.rot = Vec3(Radian(45), Radian(45), 0);
	mLightCamera.rect = RectAngle(-480, 480, 270, -270);
	mLightCamera.oFarZ = 1000.f;

	mShadowObjs.clear();
	mParents.clear();
}

void ShadowMap::Register(const uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
	{
		mShadowObjs.emplace_back();
		mParents.emplace_back();
	}
}

void ShadowMap::Init()
{
	mShadowObjs.clear();
	mParents.clear();

	mIndex = 0;
}

void ShadowMap::Update()
{
	//if (Key::GetKeyDown(DIK_Q))
	//{
	//	mBlur->scale = 1.f / 32.f;
	//	mBlur->pos = GetWindowHalfSize() / 2;
	//}
	//if (Key::GetKeyDown(DIK_E))
	//{
	//	mBlur->scale = 1.f / 8.f;
	//	mBlur->pos = GetWindowHalfSize();
	//}

	mLightCamera.pos = LightManager::GetInstance()->directionalLight.pos;

	// カメラの設定
	mLightCamera.Update();

	for (uint32_t i = 0; i < mShadowObjs.size(); i++)
	{
		mShadowObjs[i].SetCamera(&mLightCamera);
		mShadowObjs[i].Update();
	}

	CTransformShadowObj data =
	{
		mLightCamera.GetViewLookToMat() * mLightCamera.GetOrthoGrphicProjectionMat(),
		Mat4::Identity(),
	};

	mShadowMap->SetTransferBuffer(2, data);
	mShadowMap->Update();

	mBlur->Update();
}

void ShadowMap::RenderTextureSetting()
{
	mShadowMapRT->PrevDrawScene();
	for (uint32_t i = 0; i < mShadowObjs.size(); i++)
	{
		mShadowObjs[i].Draw();
	}
	mShadowMapRT->PostDrawScene();

	// 次のフレームの準備
	mIndex = 0;

	//mBlurRT->PrevDrawScene();
	//mShadowMap->Draw();
	//mBlurRT->PostDrawScene();
}

void ShadowMap::DrawModel()
{
}

void ShadowMap::DrawPostEffect()
{
	//mShadowMap->SetDrawCommands(2, 2);
	//mShadowMap->Draw();

	//mBlur->Draw();
}

void ShadowMap::Bind(Object3D& object)
{
	// からだったら
	if (mIndex >= mShadowObjs.size())
	{
		return;
	}

	// 確保したリストの先頭から順番にバインドする
	// バインドするオブジェクトの順番は処理による
	mShadowObjs[mIndex].pos = object.pos;
	mShadowObjs[mIndex].rot = object.rot;
	mShadowObjs[mIndex].scale = object.scale;

	if (object.GetModel() != nullptr)
	{
		mShadowObjs[mIndex].SetModel(object.GetModel());
	}

	if (object.GetParent())
	{
		mParents[mIndex] = *object.GetParent();
		mShadowObjs[mIndex].SetParent(&mParents[mIndex]);
	}

	mIndex++;
}

Camera ShadowMap::GetLightCamera()
{
	return mLightCamera;
}
