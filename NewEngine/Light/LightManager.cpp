#include "LightManager.h"

LightManager::LightManager() :
	constantBufferLightManager(new ConstantBuffer<ConstantBufferDataLightManager>)
{
	constantBufferLightManager->Init();
}

LightManager::~LightManager()
{
	delete constantBufferLightManager;
}

void LightManager::Init()
{
}

void LightManager::Update()
{
	for (int i = 0; i < directionalLightNum; i++)
	{
		constantBufferLightManager->constantBufferMap->
			constantBufferDirectionalLights[i].dir = directionalLights[i].dirVec;

		constantBufferLightManager->constantBufferMap->
			constantBufferDirectionalLights[i].color.x = directionalLights[i].color.r / 255;

		constantBufferLightManager->constantBufferMap->
			constantBufferDirectionalLights[i].color.y = directionalLights[i].color.g / 255;

		constantBufferLightManager->constantBufferMap->
			constantBufferDirectionalLights[i].color.z = directionalLights[i].color.b / 255;

		constantBufferLightManager->constantBufferMap->
			constantBufferDirectionalLights[i].isActive = directionalLights[i].isActive;
	}

	for (int i = 0; i < pointLightNum; i++)
	{
		constantBufferLightManager->constantBufferMap->
			constantBufferPointLights[i].pos = pointLights[i].pos;

		constantBufferLightManager->constantBufferMap->
			constantBufferPointLights[i].color.x = pointLights[i].color.r / 255;

		constantBufferLightManager->constantBufferMap->
			constantBufferPointLights[i].color.y = pointLights[i].color.g / 255;

		constantBufferLightManager->constantBufferMap->
			constantBufferPointLights[i].color.z = pointLights[i].color.b / 255;

		constantBufferLightManager->constantBufferMap->
			constantBufferPointLights[i].atten = pointLights[i].atten;

		constantBufferLightManager->constantBufferMap->
			constantBufferPointLights[i].isActive = pointLights[i].isActive;
	}

	for (int i = 0; i < spotLightNum; i++)
	{
		constantBufferLightManager->constantBufferMap->
			constantBufferSpotLights[i].vec = -spotLights[i].vec;

		constantBufferLightManager->constantBufferMap->
			constantBufferSpotLights[i].pos = spotLights[i].pos;

		constantBufferLightManager->constantBufferMap->
			constantBufferSpotLights[i].color.x = spotLights[i].color.r / 255;

		constantBufferLightManager->constantBufferMap->
			constantBufferSpotLights[i].color.y = spotLights[i].color.g / 255;

		constantBufferLightManager->constantBufferMap->
			constantBufferSpotLights[i].color.z = spotLights[i].color.b / 255;

		constantBufferLightManager->constantBufferMap->
			constantBufferSpotLights[i].atten = spotLights[i].atten;

		constantBufferLightManager->constantBufferMap->
			constantBufferSpotLights[i].factorAngleCos.x = cosf(Radian(spotLights[i].factorAngle.x));

		constantBufferLightManager->constantBufferMap->
			constantBufferSpotLights[i].factorAngleCos.y = cosf(Radian(spotLights[i].factorAngle.y));

		constantBufferLightManager->constantBufferMap->
			constantBufferSpotLights[i].isActive = spotLights[i].isActive;
	}
	circleShadow.Update();
}

void LightManager::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		3, constantBufferLightManager->constantBuffer->GetGPUVirtualAddress());

	circleShadow.Draw();
}
