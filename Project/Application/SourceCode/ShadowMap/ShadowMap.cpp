#include "ShadowMap.h"
using namespace ConstantBufferData;

ShadowMap::ShadowMap() :
	mShadowMap(std::make_unique<PostEffect>()),
	mShadowMapRT(TextureManager::GetRenderTexture("ShadowMap"))
{
	mShadowMapRT->useDepth = true;

	mShadowMap->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ShadowMap"));
	mShadowMap->AddRenderTexture(mShadowMapRT);
	mShadowMap->scale = 1.f / 8.f;
	mShadowMap->pos = GetWindowHalfSize();
	mShadowMap->AddMaterial<ConstantBuffer<CTransformShadowObj>>();

	mIndex = 0;

	lightCamera.rot = Vec3(Radian(45), Radian(45), 0);
	lightCamera.rect = RectAngle(-960, 960, 540, -540);
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

	float pitch = atan2f(lightCamera.pos.y, lightCamera.pos.z);
	float yaw = atan2f(lightCamera.pos.x, -lightCamera.pos.z);
	lightCamera.rot = Vec3(pitch, yaw, 0);

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

void ShadowMap::DrawPass()
{
	auto renderBase = RenderBase::GetInstance();

	renderBase->TransitionBufferState(mShadowMapRT->GetDepthTexture()->GetBufferResource(),
		D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

	mShadowMapRT->PrevDrawScene();
	for (uint32_t i = 0; i < mShadowObjs.size(); i++)
	{
		mShadowObjs[i].Draw();
	}
	mShadowMapRT->PostDrawScene();


	renderBase->TransitionBufferState(mShadowMapRT->GetDepthTexture()->GetBufferResource(),
		D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);

	// 次のフレームの準備
	mIndex = 0;
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
