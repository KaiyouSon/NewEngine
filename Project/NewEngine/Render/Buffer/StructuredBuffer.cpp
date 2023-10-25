#include "StructuredBuffer.h"
#include "RenderBase.h"
#include <cassert>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

StructuredBuffer::StructuredBuffer() : mBufferResource(nullptr)
{
}

void StructuredBuffer::Create(const uint32_t dataSize)
{
    mBufferResource = std::make_unique<BufferResource>();

    HRESULT result;

    // ヒーププロパティ
    D3D12_HEAP_PROPERTIES heapProp =
        CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    // リソース記述
    D3D12_RESOURCE_DESC resourceDesc =
        CD3DX12_RESOURCE_DESC::Buffer(dataSize + 0xff & ~0xff, // 256の倍数に切り上げることでパディング
            D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);       // UAVが可能なリソースフラグ

    // コミットされたリソースの作成
    result = RenderBase::GetInstance()->GetDevice()->
        CreateCommittedResource(
            &heapProp,          // ヒーププロパティ
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,      // リソースの記述
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
