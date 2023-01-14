#include "DirectionalLight.h"
#include "RenderBase.h"

//DirectionalLight DirectionalLight::current;

DirectionalLight::DirectionalLight()
{

}

DirectionalLight::~DirectionalLight()
{
	delete constantBufferDirectionalLight;
}

void DirectionalLight::Init()
{
	pos = { 0,0,0 };
	color = { 1,1,1 };
	isActive = true;
	constantBufferDirectionalLight = new ConstantBuffer<ConstantBufferDataDirectionalLight>;
	constantBufferDirectionalLight->Init();
}

void DirectionalLight::Update()
{
	constantBufferDirectionalLight->constantBufferMap->dir = pos;
	constantBufferDirectionalLight->constantBufferMap->color = color;
	constantBufferDirectionalLight->constantBufferMap->isActive = isActive == true ? 1 : 0;
}

void DirectionalLight::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		4, constantBufferDirectionalLight->constantBuffer->GetGPUVirtualAddress());
}
