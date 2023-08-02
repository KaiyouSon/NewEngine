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
	// カメラの設定
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
	// からだったら
	if (sObjShadows.empty() == true)
	{
		return;
	}

	// 確保したリストの先頭から順番にバインドする
	// バインドするオブジェクトの順番は処理による
	sObjShadows[sIndex].pos = object.pos;
	sObjShadows[sIndex].rot = object.rot;
	sObjShadows[sIndex].scale = object.scale;

	sObjShadows[sIndex].SetModel(object.GetModel());
	sIndex++;

	// 次のフレーム再バインドするために
	if (sIndex >= sObjShadows.size())
	{
		sIndex = 0;
	}
}
