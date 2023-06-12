#include "Fog.h"
using namespace ConstantBufferData;

Fog::Fog() :
	constantBufferFog_(std::make_unique<ConstantBuffer<CFog>>()),
	isActive(true), nearDis(0), farDis(50), color(Color::white)
{
	constantBufferFog_->Init();
}

void Fog::Update()
{
	constantBufferFog_->constantBufferMap->isActive = isActive;
	if (isActive == false) return;
	constantBufferFog_->constantBufferMap->nearDis = nearDis;
	constantBufferFog_->constantBufferMap->farDis = farDis;
	constantBufferFog_->constantBufferMap->color = color / 255;
	constantBufferFog_->constantBufferMap->color.a = color.a / 255;
}

void Fog::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		4, constantBufferFog_->constantBuffer->GetGPUVirtualAddress());
}
