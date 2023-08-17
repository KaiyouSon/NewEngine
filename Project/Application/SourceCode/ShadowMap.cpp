#include "ShadowMap.h"
using namespace ConstantBufferData;

void ShadowMap::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// ShadowObj用
	ShaderObjectManager::Create("ShadowMap");
	ShaderObjectManager::GetShaderObject("ShadowMap")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ShadowMap")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("ShadowMap")->CompileVertexShader(path + "ShadowMapVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("ShadowMap")->CompilePixelShader(path + "ShadowMapPS.hlsl", "main");

	// 3Dオブジェクト用
	GraphicsPipelineSetting setting =
		GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("ShadowMap");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.constantBufferViewNum = 3;
	GraphicsPipelineManager::Create(setting, "ShadowMap");

	// ShadowObj用
	ShaderObjectManager::Create("ShadowMapBlur");
	ShaderObjectManager::GetShaderObject("ShadowMapBlur")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ShadowMapBlur")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("ShadowMapBlur")->CompileVertexShader(path + "GaussianBlurVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("ShadowMapBlur")->CompilePixelShader(path + "GaussianBlurPS.hlsl", "main");

	// 3Dオブジェクト用
	setting =
		GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("ShadowMapBlur");
	setting.rtvNum = 1;
	GraphicsPipelineManager::Create(setting, "ShadowMapBlur");

}

ShadowMap::ShadowMap() :
	mShadowMap(std::make_unique<PostEffect>()),
	mShadowMapRT(TextureManager::GetRenderTexture("ShadowMap")),
	mBlur(std::make_unique<PostEffect>()),
	mBlurRT(TextureManager::GetRenderTexture("ShadowMapBlur"))
{
	mShadowMapRT->useDepth = true;

	mShadowMap->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("ShadowMap"));
	mShadowMap->AddRenderTexture(mShadowMapRT);
	//mShadowMap->scale = 1.f / 32.f;
	//mShadowMap->pos = GetWindowHalfSize() / 2;
	mShadowMap->scale = 1.f / 2.f;
	mShadowMap->pos = GetWindowHalfSize();
	mShadowMap->AddMaterial(ConstantBuffer<CTransformShadowObj>{});

	mBlur->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("ShadowMapBlur"));
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

}

void ShadowMap::Update()
{
	if (Key::GetKeyDown(DIK_Q))
	{
		mBlur->scale = 1.f / 32.f;
		mBlur->pos = GetWindowHalfSize() / 2;
	}
	if (Key::GetKeyDown(DIK_E))
	{
		mBlur->scale = 1.f / 8.f;
		mBlur->pos = GetWindowHalfSize();
	}

	mLightCamera.pos = LightManager::GetInstance()->directionalLight.pos;

	// カメラの設定
	mLightCamera.Update();

	for (uint32_t i = 0; i < mShadowObjs.size(); i++)
	{
		mShadowObjs[i].SetCamera(&mLightCamera);
		mShadowObjs[i].Update();
		if (i > mIndex)
		{
			break;
		}
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
		if (i > mIndex)
		{
			break;
		}
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
