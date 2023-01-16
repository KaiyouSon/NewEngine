#include "PointLight.h"

PointLight::PointLight() :
	pos(0, 0, 0), color(Color::white), atten(1, 1, 1), isActive(false)
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
	constantBufferPointLight->constantBufferMap->color.x = color.r / 255;
	constantBufferPointLight->constantBufferMap->color.y = color.g / 255;
	constantBufferPointLight->constantBufferMap->color.z = color.b / 255;
	constantBufferPointLight->constantBufferMap->atten = atten;
	constantBufferPointLight->constantBufferMap->isActive = isActive == true ? 1 : 0;
	//constantBufferPointLight->constantBufferMap->isActive = 1;
}

void PointLight::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		5, constantBufferPointLight->constantBuffer->GetGPUVirtualAddress());
}
