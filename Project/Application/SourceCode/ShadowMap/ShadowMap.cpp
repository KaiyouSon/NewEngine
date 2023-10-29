#include "ShadowMap.h"
using namespace ConstantBufferData;

ShadowMap::ShadowMap() :
	mShadowMap(std::make_unique<PostEffect>()),
	mShadowMapRT(TextureManager::GetRenderTexture("ShadowMap"))
{
	mShadowMapRT->useDepth = true;

	mShadowMap->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ShadowMap"));
	mShadowMap->AddRenderTexture(mShadowMapRT);
	mShadowMap->scale = 1.f / 32.f;
	mShadowMap->pos = GetWindowHalfSize();
	mShadowMap->AddMaterial<ConstantBuffer<CTransformShadowObj>>();

	mIndex = 0;

	lightCamera.rot = Vec3(Radian(45), Radian(45), 0);
	lightCamera.rect = RectAngle(-480, 480, 270, -270);
	lightCamera.oFarZ = 1000.f;

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

void ShadowMap::Update(const Vec3 lightPos)
{
	lightCamera.pos = lightPos;

	// カメラの設定
	lightCamera.Update();

	for (uint32_t i = 0; i < mShadowObjs.size(); i++)
	{
		mShadowObjs[i].SetCamera(&lightCamera);
		mShadowObjs[i].Update();
	}

	CTransformShadowObj data =
	{
		lightCamera.GetViewLookToMat() * lightCamera.GetOrthoGrphicProjectionMat(),
		Mat4::Identity(),
	};

	mShadowMap->SetTransferBuffer(2, data);
	mShadowMap->Update();
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
}

void ShadowMap::DrawModel()
{
}

void ShadowMap::DrawPostEffect()
{
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
	return lightCamera;
}
