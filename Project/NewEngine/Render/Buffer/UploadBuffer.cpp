#include "UploadBuffer.h"
#include "RenderBase.h"
#include <cassert>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

UploadBuffer::UploadBuffer() :
	mResult(HRESULT()), mBufferResource(std::make_unique<BufferResource>())
{
}

void UploadBuffer::Create(const uint64_t uploadSize)
{
	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer(uploadSize);

	// バッファ生成
	mResult = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mBufferResource->buffer));
	assert(SUCCEEDED(mResult));

	mBufferResource->bufferState = D3D12_RESOURCE_STATE_GENERIC_READ;
	mBufferResource->buffer->SetName(L"UploadBuffer");
}

BufferResource* UploadBuffer::GetBufferResource()
{
	return mBufferResource.get();
}
