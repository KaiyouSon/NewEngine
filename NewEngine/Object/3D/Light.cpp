#include "Light.h"
#include "RenderBase.h"

Light Light::current;

Light::Light()
{

}

Light::~Light()
{
	delete constantBufferLight;
}

void Light::Init()
{
	lightPos = { 0,0,0 };
	lightColor = { 1,1,1 };
	constantBufferLight = new ConstantBuffer<ConstantBufferDataLight>;
	constantBufferLight->Init();
}

void Light::Update()
{
	constantBufferLight->constantBufferMap->dir = lightPos;
	constantBufferLight->constantBufferMap->color = lightColor;
}

void Light::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		4, constantBufferLight->constantBuffer->GetGPUVirtualAddress());
}
