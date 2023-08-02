#include "ShadowMap.h"

std::vector<Object3D> ShadowMap::sObjShadows;
uint32_t ShadowMap::sIndex;

ShadowMap::ShadowMap() :
	currentScene_(std::make_unique<PostEffect>()),
	renderTex_(TextureManager::GetRenderTexture("CurrentScene"))
{
	renderTex_->useDepth = true;

	currentScene_->AddRenderTexture(renderTex_);
	currentScene_->anchorPoint = 0;
	currentScene_->scale = 0.5f;
	currentScene_->pos = GetWindowHalfSize() / 2;

	sIndex = 0;
}

void ShadowMap::Init()
{
}

void ShadowMap::Update()
{
	// �J�����̐ݒ�
	lightCamera_.pos = LightManager::GetInstance()->directionalLight.pos;
	lightCamera_.rot = Vec3(Radian(90), 0, 0);
	lightCamera_.Update();

	for (auto& obj : sObjShadows)
	{
		obj.SetCamera(&lightCamera_);
		obj.Update();
	}

	currentScene_->Update();
}

void ShadowMap::RenderTextureSetting()
{
	renderTex_->PrevDrawScene();

	RenderBase::GetInstance()->SetObject3DDrawCommand();
	for (auto& obj : sObjShadows)
	{
		obj.Draw();
	}

	renderTex_->PostDrawScene();
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
	currentScene_->Draw();
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
