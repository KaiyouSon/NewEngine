#include "ShadowMap.h"

std::vector<Object3D> ShadowMap::sObjShadows;
std::vector<Transform> ShadowMap::sParents;
uint32_t ShadowMap::sIndex;
Camera ShadowMap::sLightCamera;

ShadowMap::ShadowMap() :
	mCurrentScene(std::make_unique<PostEffect>()),
	mRenderTex(TextureManager::GetRenderTexture("CurrentScene"))
{
	mRenderTex->useDepth = true;

	mCurrentScene->AddRenderTexture(mRenderTex);
	mCurrentScene->scale = 0.25f;
	mCurrentScene->pos = GetWindowHalfSize() / 4;
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

	for (auto& obj : sObjShadows)
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

	for (auto& obj : sObjShadows)
	{
		obj.Draw();
	}

	mRenderTex->PostDrawScene();
}

void ShadowMap::DrawModel()
{
	for (auto& obj : sObjShadows)
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
	sObjShadows.emplace_back();
	sParents.emplace_back();
}

void ShadowMap::Bind(Object3D& object)
{
	// ���炾������
	if (sObjShadows.empty() == true)
	{
		return;
	}

	// �m�ۂ������X�g�̐擪���珇�ԂɃo�C���h����
	// �o�C���h����I�u�W�F�N�g�̏��Ԃ͏����ɂ��
	sObjShadows[sIndex].pos = object.pos;
	sObjShadows[sIndex].rot = object.rot;
	sObjShadows[sIndex].scale = object.scale;

	if (object.GetModel() != nullptr)
	{
		sObjShadows[sIndex].SetModel(object.GetModel());
	}

	if (object.GetParent())
	{
		sParents[sIndex] = *object.GetParent();
		sObjShadows[sIndex].SetParent(&sParents[sIndex]);
	}

	sIndex++;

	// ���̃t���[���ăo�C���h���邽�߂�
	if (sIndex >= sObjShadows.size())
	{
		sIndex = 0;
	}
}

Camera ShadowMap::GetLightCamera()
{
	return sLightCamera;
}
