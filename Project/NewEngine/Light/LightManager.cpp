#include "LightManager.h"
#include "ConstantBufferData.h"
using namespace ConstantBufferData;

LightManager::LightManager()
{
	Init();
}

void LightManager::Init()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CDirectionalLight>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	material_.Init();
}

void LightManager::Update()
{
	// マトリックス
	CDirectionalLight directionalLightData;
	directionalLightData.color = directionalLight.color.To01();
	directionalLightData.dir = directionalLight.pos.Norm();
	directionalLightData.isActive = directionalLight.isActive;
	TransferDataToConstantBuffer(material_.constantBuffers[0].get(), directionalLightData);
}
void LightManager::DrawCommand(const uint32_t index)
{
	RenderBase* renderBase = RenderBase::GetInstance();

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		index, material_.constantBuffers[0]->bufferResource->buffer->GetGPUVirtualAddress());
}
