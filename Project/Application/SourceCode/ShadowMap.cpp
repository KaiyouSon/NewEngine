#include "ShadowMap.h"
using namespace ConstantBufferData;

std::vector<ShadowObj> ShadowMap::sShadowObjs;
std::vector<Transform> ShadowMap::sParents;
uint32_t ShadowMap::sIndex;
Camera ShadowMap::sLightCamera;

void ShadowMap::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// ShadowObj�p
	ShaderObjectManager::Create("ShadowMap");
	ShaderObjectManager::GetShaderObject("ShadowMap")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ShadowMap")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("ShadowMap")->CompileVertexShader(path + "ShadowMapVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("ShadowMap")->CompilePixelShader(path + "ShadowMapPS.hlsl", "main");

	// 3D�I�u�W�F�N�g�p
	GraphicsPipelineSetting setting =
		GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("ShadowMap");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.constantBufferViewNum = 3;
	GraphicsPipelineManager::Create(setting, "ShadowMap");

}

ShadowMap::ShadowMap() :
	mShadowMap(std::make_unique<PostEffect>()),
	mRenderTex(TextureManager::GetRenderTexture("ShadowMap"))
{
	mRenderTex->useDepth = true;

	mShadowMap->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("ShadowMap"));
	mShadowMap->AddRenderTexture(mRenderTex);
	mShadowMap->scale = 1.f / 32.f;
	mShadowMap->pos = GetWindowHalfSize() / 2;
	mShadowMap->AddMaterial(ConstantBuffer<CTransformShadowObj>{});

	sIndex = 0;

	sLightCamera.rot = Vec3(Radian(45), Radian(45), 0);
	sLightCamera.rect = RectAngle(-480, 480, 270, -270);
	sLightCamera.oFarZ = 1000.f;
}

void ShadowMap::Init()
{
}

void ShadowMap::Update()
{
	sLightCamera.pos = LightManager::GetInstance()->directionalLight.pos;

	// �J�����̐ݒ�
	sLightCamera.Update();

	for (auto& obj : sShadowObjs)
	{
		obj.SetCamera(&sLightCamera);
		obj.Update();
	}

	CTransformShadowObj data =
	{
		sLightCamera.GetViewLookToMat() * sLightCamera.GetOrthoGrphicProjectionMat(),
		Mat4::Identity(),
	};

	mShadowMap->SetTransferBuffer(2, data);
	mShadowMap->Update();
}

void ShadowMap::RenderTextureSetting()
{
	mRenderTex->PrevDrawScene();

	for (auto& obj : sShadowObjs)
	{
		obj.Draw();
	}

	mRenderTex->PostDrawScene();
}

void ShadowMap::DrawModel()
{
	for (auto& obj : sShadowObjs)
	{
		obj.Draw();
	}
}

void ShadowMap::DrawPostEffect()
{
	mShadowMap->SetDrawCommands(2, 2);
	mShadowMap->Draw();
}

void ShadowMap::Register()
{
	sShadowObjs.emplace_back();
	sParents.emplace_back();
}

void ShadowMap::Bind(Object3D& object)
{
	// ���炾������
	if (sShadowObjs.empty() == true)
	{
		return;
	}

	// �m�ۂ������X�g�̐擪���珇�ԂɃo�C���h����
	// �o�C���h����I�u�W�F�N�g�̏��Ԃ͏����ɂ��
	sShadowObjs[sIndex].pos = object.pos;
	sShadowObjs[sIndex].rot = object.rot;
	sShadowObjs[sIndex].scale = object.scale;

	if (object.GetModel() != nullptr)
	{
		sShadowObjs[sIndex].SetModel(object.GetModel());
	}

	if (object.GetParent())
	{
		sParents[sIndex] = *object.GetParent();
		sShadowObjs[sIndex].SetParent(&sParents[sIndex]);
	}

	sIndex++;

	// ���̃t���[���ăo�C���h���邽�߂�
	if (sIndex >= sShadowObjs.size())
	{
		sIndex = 0;
	}
}

Camera ShadowMap::GetLightCamera()
{
	return sLightCamera;
}
