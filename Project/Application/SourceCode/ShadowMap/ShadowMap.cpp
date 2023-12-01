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

	rectLeftTop = Vec2(-400, 100.f);
	rectSize = Vec2(624.f, 351.f);
	lightCamera.rot = Vec3(Radian(45), Radian(170), 0);
	lightCamera.rect = RectAngle(rectLeftTop, rectLeftTop + rectSize * Vec2(1, -1));
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

void ShadowMap::LightViewUpdate(const Vec3 lightPos)
{
	lightCamera.pos = lightPos;

	rectLeftTop = Vec2(-400, 100.f);
	lightCamera.rect = RectAngle(rectLeftTop, rectLeftTop + rectSize * Vec2(1, -1));

	// カメラの設定
	lightCamera.Update();
}

void ShadowMap::Update()
{
	//float pitch = atan2f(lightCamera.pos.y, lightCamera.pos.z);
	//float yaw = atan2f(lightCamera.pos.x, -lightCamera.pos.z);
	//lightCamera.rot = Vec3(pitch, 0, 0);

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
	static bool flag = false;

	if (Key::GetKeyDown(DIK_M))
	{
		flag = flag == true ? false : true;
	}

	if (flag == true)
	{
		mShadowMap->Draw();
	}
}

void ShadowMap::DrawDebugGui()
{
	Gui::BeginWindow("ShadowMap");
	Gui::DrawImage(mShadowMapRT->GetDepthTexture(), Vec2(960, 540) * 0.75f);

	Gui::DrawSlider2("Rect LeftTop", rectLeftTop);
	Gui::DrawSlider2("Rect Size", rectSize);
	Vec3 angle = Angle(lightCamera.rot);
	Gui::DrawSlider3("LightView Angle", angle);
	lightCamera.rot = Radian(angle);

	Gui::DrawSlider1("LightView oNearZ", lightCamera.oNearZ);
	Gui::DrawSlider1("LightView oFarZ", lightCamera.oFarZ);

	Gui::EndWindow();
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
