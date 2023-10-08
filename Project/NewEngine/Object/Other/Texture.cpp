#include "Texture.h"
#include "RenderBase.h"
#include <cassert>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

Texture::Texture() :
	mUploadBuffer(std::make_unique<UploadBuffer>()), mInitalSize(0)
{
}

void Texture::Create(const D3D12_RESOURCE_DESC& resourceDesc, uint32_t mipLevels)
{
	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// サイズ設定
	mInitalSize = Vec2((float)resourceDesc.Width, (float)resourceDesc.Height);

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
	mBufferResource->buffer->SetName(L"TextureBuffer");

	// アップロードサイズを取得
	uint64_t uploadSize =
		GetRequiredIntermediateSize(mBufferResource->buffer.Get(), 0, mipLevels);

	// アップロードバッファの生成
	mUploadBuffer->Create(uploadSize);
}

UploadBuffer* Texture::GetUploadBuffer()
{
	return mUploadBuffer.get();
}

Vec2 Texture::GetInitalSize()
{
	return mInitalSize;
}
