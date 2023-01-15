#include "SpotLight.h"

SpotLight::SpotLight() :
	constantBufferSpotLight(new ConstantBuffer<ConstantBufferDataSpotLight>),
	vec(Vec3::right), pos(0), color(1), atten(1), factorAngleCos(0.5f, 0.2f), isActive(false)
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
	constantBufferSpotLight->constantBufferMap->color = color;
	constantBufferSpotLight->constantBufferMap->atten = atten;
	constantBufferSpotLight->constantBufferMap->factorAngleCos = factorAngleCos;
	constantBufferSpotLight->constantBufferMap->isActive = isActive;
}

void SpotLight::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// �}�e���A���ƃg�����X�t�H�[����CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		6, constantBufferSpotLight->constantBuffer->GetGPUVirtualAddress());
}
