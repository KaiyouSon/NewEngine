#include "PointLight.h"

PointLight::PointLight() :
	pos(0, 0, 0), color(1, 1, 1), atten(1, 1, 1), isActive(false)
{
}

PointLight::~PointLight()
{
	delete constantBufferPointLight;
}

void PointLight::Init()
{
	constantBufferPointLight = new ConstantBuffer<ConstantBufferDataPointLight>;
	constantBufferPointLight->Init();
}

void PointLight::Update()
{
	constantBufferPointLight->constantBufferMap->pos = pos;
	constantBufferPointLight->constantBufferMap->color = color;
	constantBufferPointLight->constantBufferMap->atten = atten;
	constantBufferPointLight->constantBufferMap->active = isActive;
}

void PointLight::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		5, constantBufferPointLight->constantBuffer->GetGPUVirtualAddress());
}
