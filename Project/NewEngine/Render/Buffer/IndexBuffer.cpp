#include "IndexBuffer.h"
#include "RenderBase.h"
#include <cassert>

IndexBuffer::IndexBuffer() : mBufferResource(nullptr)
{
}

void IndexBuffer::Create(const std::vector<uint16_t>& indices)
{
	mBufferResource = std::make_unique<BufferResource>();

	// インデックスデータのサイズを計算
	uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint16_t) * indices.size());

	// アップロードヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUによって読み取り可能なヒープ

	// リソース記述
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // インデックスデータのサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT result;

	// コミットされたリソースの作成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapProp, // ヒーププロパティ
			D3D12_HEAP_FLAG_NONE,
			&resDesc, // リソース記述
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mBufferResource->buffer));
	assert(SUCCEEDED(result));

	mBufferResource->bufferState = D3D12_RESOURCE_STATE_GENERIC_READ;
	mBufferResource->buffer->SetName(L"IndexBuffer");

	// リソースをマップしてデータをコピー
	uint16_t* indexMap = nullptr;
	result = mBufferResource->buffer->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// インデックスデータをコピー
	for (int i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i]; // メモリにコピー
	}
	// アンマップ
	mBufferResource->buffer->Unmap(0, nullptr);

	// リソースビューを設定
	mBufferView.BufferLocation = mBufferResource->buffer->GetGPUVirtualAddress();
	mBufferView.Format = DXGI_FORMAT_R16_UINT;
	mBufferView.SizeInBytes = sizeIB;
}

D3D12_INDEX_BUFFER_VIEW* IndexBuffer::GetibViewAddress()
{
	return &mBufferView;
}