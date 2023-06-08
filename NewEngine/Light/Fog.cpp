#include "Fog.h"

Fog::Fog() :
	constantBufferFog(new ConstantBuffer<ConstantBufferDataFog>),
	isActive(true), nearDis(0), farDis(50), color(Color::sWhite)
{
	constantBufferFog->Init();
}

Fog::~Fog()
{
	delete constantBufferFog;
}

void Fog::Update()
{
	constantBufferFog->constantBufferMap->isActive = isActive;
	if (isActive == false) return;
	constantBufferFog->constantBufferMap->nearDis = nearDis;
	constantBufferFog->constantBufferMap->farDis = farDis;
	constantBufferFog->constantBufferMap->color = color / 255;
	constantBufferFog->constantBufferMap->color.a = color.a / 255;
}

void Fog::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		4, constantBufferFog->constantBuffer->GetGPUVirtualAddress());
}
