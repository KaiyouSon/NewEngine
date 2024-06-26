#include "VolumeTexture.h"
#include "RenderBase.h"
#include <cassert>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

void VolumeTexture::Create(const D3D12_RESOURCE_DESC& resourceDesc)
{
	mType = TextureType::Volume;

	mUploadBuffer = std::make_unique<UploadBuffer>();

	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// バッファ生成
	mResult = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&mBufferResource->buffer));
	assert(SUCCEEDED(mResult));

	// 名前設定
	mBufferResource->buffer->SetName(L"VolumeTextureBuffer");
	mBufferResource->bufferState = D3D12_RESOURCE_STATE_COPY_DEST;

	// アップロードサイズを取得
	uint64_t uploadSize =
		GetRequiredIntermediateSize(mBufferResource->buffer.Get(), 0, 1);

	// UploadBufferの生成
	mUploadBuffer->Create(static_cast<uint32_t>(uploadSize));
}

UploadBuffer* VolumeTexture::GetUploadBuffer()
{
	return mUploadBuffer.get();
}
