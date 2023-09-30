#include "CircleShadow.h"
using namespace ConstantBufferData;

CircleShadow::CircleShadow() :
	constantBufferCircleShadow_(std::make_unique<ConstantBuffer<CCircleShadow>>()),
	vec(Vec3::right), pos(0), disCasterLight(100.f), atten(0.5f, 0.6f, 0.0f),
	factorAngleCos(0.2f, 0.5f), isActive(false)
{
	constantBufferCircleShadow_->Create();
}

void CircleShadow::Update()
{
	constantBufferCircleShadow_->constantBufferMap->vec = -vec;
	constantBufferCircleShadow_->constantBufferMap->pos = pos;
	constantBufferCircleShadow_->constantBufferMap->disCasterLight = disCasterLight;
	constantBufferCircleShadow_->constantBufferMap->atten = atten;
	constantBufferCircleShadow_->constantBufferMap->factorAngleCos.x = cosf(Radian(factorAngleCos.x));
	constantBufferCircleShadow_->constantBufferMap->factorAngleCos.y = cosf(Radian(factorAngleCos.y));
	constantBufferCircleShadow_->constantBufferMap->isActive = isActive;
}

void CircleShadow::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		6, constantBufferCircleShadow_->bufferResource->buffer->GetGPUVirtualAddress());
}
