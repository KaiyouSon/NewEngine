#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer() : mBufferResource(nullptr)
{
}

void StructuredBuffer::Create(const uint32_t dataSize)
{
	mBufferResource = std::make_unique<BufferResource>();

	HRESULT result;

	// 繝偵・繝励・險ｭ螳・
	D3D12_HEAP_PROPERTIES heapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// 繝ｪ繧ｽ繝ｼ繧ｹ險ｭ螳・
	D3D12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer(dataSize + 0xff & ~0xff,	// 256繝舌う繝医い繝ｩ繧､繝ｳ繝｡繝ｳ繝・
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);			// UAV菴ｿ逕ｨ繧定ｨｱ蜿ｯ

	// 繝舌ャ繝輔ぃ縺ｮ逕滓・
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapProp,	// 繝偵・繝励・險ｭ螳・
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc, // 繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ險ｭ螳・
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mBufferResource->buffer));
	assert(SUCCEEDED(result));

	mBufferResource->bufferState = D3D12_RESOURCE_STATE_GENERIC_READ;
	mBufferResource->buffer->SetName(L"StructuredBuffer");
}

BufferResource* StructuredBuffer::GetBufferResource()
{
	return mBufferResource.get();
}
