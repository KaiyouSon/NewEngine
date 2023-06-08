#include "CircleShadow.h"

CircleShadow::CircleShadow() :
	constantBufferCircleShadow(new ConstantBuffer<ConstantBufferDataCircleShadow>),
	vec(Vec3::right), pos(0), disCasterLight(100.f), atten(0.5f, 0.6f, 0.0f),
	factorAngleCos(0.2f, 0.5f), isActive(false)
{
	constantBufferCircleShadow->Init();
}

CircleShadow::~CircleShadow()
{
	delete constantBufferCircleShadow;
}

void CircleShadow::Update()
{
	constantBufferCircleShadow->constantBufferMap->vec = -vec;
	constantBufferCircleShadow->constantBufferMap->pos = pos;
	constantBufferCircleShadow->constantBufferMap->disCasterLight = disCasterLight;
	constantBufferCircleShadow->constantBufferMap->atten = atten;
	constantBufferCircleShadow->constantBufferMap->factorAngleCos.x = cosf(Radian(factorAngleCos.x));
	constantBufferCircleShadow->constantBufferMap->factorAngleCos.y = cosf(Radian(factorAngleCos.y));
	constantBufferCircleShadow->constantBufferMap->isActive = isActive;
}

void CircleShadow::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		6, constantBufferCircleShadow->constantBuffer->GetGPUVirtualAddress());
}
