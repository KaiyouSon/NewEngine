#include "Texture.h"
#include "RenderBase.h"
#include <cassert>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

Texture::Texture() 
{

}
Texture::Texture(const std::string& tag)
{
	mTag = tag;
}

// テクスチャーのバッファ生成する関数
void Texture::Create(const D3D12_RESOURCE_DESC& resourceDesc, uint32_t mipLevels)
{
	mType = TextureType::Default;

	mBufferResource = std::make_unique<BufferResource>();
	mUploadBuffer = std::make_unique<UploadBuffer>();

	// サイズ設定
	mInitalSize = Vec2((float)resourceDesc.Width, (float)resourceDesc.Height);

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
	mBufferResource->buffer->SetName(L"TextureBuffer");
	mBufferResource->bufferState = D3D12_RESOURCE_STATE_COPY_DEST;

	// アップロードサイズを取得
	uint64_t uploadSize =
		GetRequiredIntermediateSize(mBufferResource->buffer.Get(), 0, mipLevels);

	// アップロードバッファの生成
	mUploadBuffer->Create(uploadSize);

}

void Texture::SetScratchImage(DirectX::ScratchImage* scratchImage)
{
	mScratchImage = std::move(*scratchImage);
}

UploadBuffer* Texture::GetUploadBuffer()
{
	return mUploadBuffer.get();
}

DirectX::ScratchImage* Texture::GetScratchImage()
{
	return &mScratchImage;
}
