#include "Light.h"
#include "RenderBase.h"

Light* Light::current = new Light;

Light::Light()
{

}

Light::~Light()
{
	delete constantBufferLight;
}

void Light::Init()
{
	lightDir = { 0,1,1 };
	lightColor = { 1,1,1 };
	constantBufferLight = new ConstantBuffer<ConstantBufferDataLight>;
	constantBufferLight->Init();
}

void Light::Update()
{
	constantBufferLight->constantBufferMap->dir = -lightDir;
	constantBufferLight->constantBufferMap->color = lightColor;
}

void Light::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// �}�e���A���ƃg�����X�t�H�[����CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		4, constantBufferLight->constantBuffer->GetGPUVirtualAddress());
}
