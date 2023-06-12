#include "LightManager.h"

bool LightManager::isPointLighting = false;
bool LightManager::isSpotLighting = false;

LightManager::LightManager() :
	constantBufferLightManager_(std::make_unique<ConstantBuffer<ConstantBufferDataLightManager>>())
{
	constantBufferLightManager_->Init();
}

void LightManager::Init()
{
}

void LightManager::Update()
{
	DirectionalLightUpdate();

	if (isPointLighting == true)
	{
		PointLightUpdate();
	}

	if (isSpotLighting == true)
	{
		SpotLightUpdate();
	}

	fog.Update();
	circleShadow.Update();
}
void LightManager::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		3, constantBufferLightManager_->constantBuffer->GetGPUVirtualAddress());

	fog.Draw();
	circleShadow.Draw();
}


void LightManager::DirectionalLightUpdate()
{
	for (uint32_t i = 0; i < sDirectionalLightNum_; i++)
	{
		constantBufferLightManager_->constantBufferMap->
			constantBufferDirectionalLights[i].dir = directionalLights[i].dirVec.Norm();

		constantBufferLightManager_->constantBufferMap->
			constantBufferDirectionalLights[i].color.x = directionalLights[i].color.r / 255;

		constantBufferLightManager_->constantBufferMap->
			constantBufferDirectionalLights[i].color.y = directionalLights[i].color.g / 255;

		constantBufferLightManager_->constantBufferMap->
			constantBufferDirectionalLights[i].color.z = directionalLights[i].color.b / 255;

		constantBufferLightManager_->constantBufferMap->
			constantBufferDirectionalLights[i].isActive = directionalLights[i].isActive;
	}
}

void LightManager::PointLightUpdate()
{
	for (uint32_t i = 0; i < sPointLightNum_; i++)
	{
		constantBufferLightManager_->constantBufferMap->
			constantBufferPointLights[i].pos = pointLights[i].pos;

		constantBufferLightManager_->constantBufferMap->
			constantBufferPointLights[i].color.x = pointLights[i].color.r / 255;

		constantBufferLightManager_->constantBufferMap->
			constantBufferPointLights[i].color.y = pointLights[i].color.g / 255;

		constantBufferLightManager_->constantBufferMap->
			constantBufferPointLights[i].color.z = pointLights[i].color.b / 255;

		constantBufferLightManager_->constantBufferMap->
			constantBufferPointLights[i].atten = pointLights[i].atten;

		constantBufferLightManager_->constantBufferMap->
			constantBufferPointLights[i].isActive = pointLights[i].isActive;
	}
}

void LightManager::SpotLightUpdate()
{
	for (uint32_t i = 0; i < sSpotLightNum_; i++)
	{
		constantBufferLightManager_->constantBufferMap->
			constantBufferSpotLights[i].vec = -spotLights[i].vec;

		constantBufferLightManager_->constantBufferMap->
			constantBufferSpotLights[i].pos = spotLights[i].pos;

		constantBufferLightManager_->constantBufferMap->
			constantBufferSpotLights[i].color.x = spotLights[i].color.r / 255;

		constantBufferLightManager_->constantBufferMap->
			constantBufferSpotLights[i].color.y = spotLights[i].color.g / 255;

		constantBufferLightManager_->constantBufferMap->
			constantBufferSpotLights[i].color.z = spotLights[i].color.b / 255;

		constantBufferLightManager_->constantBufferMap->
			constantBufferSpotLights[i].atten = spotLights[i].atten;

		constantBufferLightManager_->constantBufferMap->
			constantBufferSpotLights[i].factorAngleCos.x = cosf(Radian(spotLights[i].factorAngle.x));

		constantBufferLightManager_->constantBufferMap->
			constantBufferSpotLights[i].factorAngleCos.y = cosf(Radian(spotLights[i].factorAngle.y));

		constantBufferLightManager_->constantBufferMap->
			constantBufferSpotLights[i].isActive = spotLights[i].isActive;
	}
}