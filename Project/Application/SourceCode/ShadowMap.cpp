#include "ShadowMap.h"

std::vector<ShadowObj> ShadowMap::sShadowObjs;
std::vector<Transform> ShadowMap::sParents;
uint32_t ShadowMap::sIndex;
Camera ShadowMap::sLightCamera;

ShadowMap::ShadowMap() :
	mCurrentScene(std::make_unique<PostEffect>()),
	mRenderTex(TextureManager::GetRenderTexture("CurrentScene"))
{
	mRenderTex->useDepth = true;

	mCurrentScene->AddRenderTexture(mRenderTex);
	mCurrentScene->scale = 0.125f;
	mCurrentScene->pos = GetWindowHalfSize() / 2;
	sIndex = 0;
}

void ShadowMap::Init()
{
}

void ShadowMap::Update()
{
	// �J�����̐ݒ�
	sLightCamera.Update();

	static uint32_t i = 0;

	for (auto& obj : sShadowObjs)
	{
		obj.SetCamera(&sLightCamera);
		obj.Update();
		i++;
	}
	mCurrentScene->Update();
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
	mCurrentScene->Draw();
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
