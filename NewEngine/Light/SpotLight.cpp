#include "SpotLight.h"

SpotLight::SpotLight() :
	constantBufferSpotLight(new ConstantBuffer<ConstantBufferDataSpotLight>),
	vec(Vec3::right), pos(0), color(Color::white), atten(1), factorAngleCos(0.5f, 0.2f), isActive(false)
{
	constantBufferSpotLight->Init();
}

SpotLight::~SpotLight()
{
	delete constantBufferSpotLight;
}

void SpotLight::Init()
{
}

void SpotLight::Update()
{
	constantBufferSpotLight->constantBufferMap->vec = -vec;
	constantBufferSpotLight->constantBufferMap->pos = pos;
	constantBufferSpotLight->constantBufferMap->color.x = color.r / 255;
	constantBufferSpotLight->constantBufferMap->color.y = color.g / 255;
	constantBufferSpotLight->constantBufferMap->color.z = color.b / 255;
	constantBufferSpotLight->constantBufferMap->atten = atten;
	constantBufferSpotLight->constantBufferMap->factorAngleCos.x = cosf(Radian(factorAngleCos.x));
	constantBufferSpotLight->constantBufferMap->factorAngleCos.y = cosf(Radian(factorAngleCos.y));
	constantBufferSpotLight->constantBufferMap->isActive = isActive;
}

void SpotLight::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		6, constantBufferSpotLight->constantBuffer->GetGPUVirtualAddress());
}
