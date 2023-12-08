#include "DistanceFog.h"
#include "Material.h"
using namespace ConstantBufferData;

DistanceFog::DistanceFog() :
	mMaterial(std::make_unique<Material>())
{
	MaterialInit();

	data.color = Color::white;
	data.isActive = true;
	data.nearDis = 30.f;
	data.farDis = 100.f;
}

void DistanceFog::TransferData()
{
	CDistanceFog transfer = data;
	transfer.color.To01();

	// データ転送
	TransferDataToConstantBuffer(mMaterial->constantBuffers[0].get(), transfer);
}

void DistanceFog::DrawCommands(const uint32_t index)
{
	RenderBase* renderBase = RenderBase::GetInstance();

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		index, mMaterial->constantBuffers[0]->bufferResource->buffer->GetGPUVirtualAddress());
}

void DistanceFog::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 距離フォグのデータ
	iConstantBuffer = std::make_unique<ConstantBuffer<CDistanceFog>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	mMaterial->Init();
}