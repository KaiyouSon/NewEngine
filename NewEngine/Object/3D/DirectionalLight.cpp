#include "DirectionalLight.h"
#include "RenderBase.h"

DirectionalLight::DirectionalLight() :
	pos(0, 1, 0), color(Color::white), isActive(false)
{

}

DirectionalLight::~DirectionalLight()
{
	delete constantBufferDirectionalLight;
}

void DirectionalLight::Init()
{
	constantBufferDirectionalLight = new ConstantBuffer<ConstantBufferDataDirectionalLight>;
	constantBufferDirectionalLight->Init();
}

void DirectionalLight::Update()
{
	constantBufferDirectionalLight->constantBufferMap->dir = pos;
	constantBufferDirectionalLight->constantBufferMap->color.x = color.r / 255;
	constantBufferDirectionalLight->constantBufferMap->color.y = color.g / 255;
	constantBufferDirectionalLight->constantBufferMap->color.z = color.b / 255;
	constantBufferDirectionalLight->constantBufferMap->isActive = isActive == true ? 1 : 0;
}

void DirectionalLight::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		4, constantBufferDirectionalLight->constantBuffer->GetGPUVirtualAddress());
}
