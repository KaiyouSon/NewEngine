#include "ShadowMap.h"

std::vector<Object3D> ShadowMap::sObjShadows;
uint32_t ShadowMap::sIndex;

ShadowMap::ShadowMap() :
	mCurrentScene(std::make_unique<PostEffect>()),
	mRenderTex(TextureManager::GetRenderTexture("CurrentScene"))
{
	mRenderTex->useDepth = true;

	mCurrentScene->AddRenderTexture(mRenderTex);
	mCurrentScene->anchorPoint = 0;
	mCurrentScene->scale = 0.5f;
	mCurrentScene->pos = GetWindowHalfSize() / 2;

	sIndex = 0;
}

void ShadowMap::Init()
{
}

void ShadowMap::Update()
{
	// �J�����̐ݒ�
	mLightCamera.pos = LightManager::GetInstance()->directionalLight.pos;
	mLightCamera.rot = Vec3(Radian(90), 0, 0);
	mLightCamera.Update();

	for (auto& obj : sObjShadows)
	{
		obj.SetCamera(&mLightCamera);
		obj.Update();
	}

	mCurrentScene->Update();
}

void ShadowMap::RenderTextureSetting()
{
	mRenderTex->PrevDrawScene();

	RenderBase::GetInstance()->SetObject3DDrawCommand();
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
	sObjShadows.emplace_back(Object3D());
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

	sObjShadows[sIndex].SetModel(object.GetModel());
	sIndex++;

	// ���̃t���[���ăo�C���h���邽�߂�
	if (sIndex >= sObjShadows.size())
	{
		sIndex = 0;
	}
}
