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
	// カメラの設定
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
	// からだったら
	if (sShadowObjs.empty() == true)
	{
		return;
	}

	// 確保したリストの先頭から順番にバインドする
	// バインドするオブジェクトの順番は処理による
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

	// 次のフレーム再バインドするために
	if (sIndex >= sShadowObjs.size())
	{
		sIndex = 0;
	}
}

Camera ShadowMap::GetLightCamera()
{
	return sLightCamera;
}
