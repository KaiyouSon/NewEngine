#include "Texture.h"
#include "RenderBase.h"
#include <cassert>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

Texture::Texture()
{

}
Texture::Texture(const std::string tag, const std::string& path)
{
	mTag = tag;
	mPath = path;
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

void Texture::Create(DirectX::ScratchImage& scratchImage, const DirectX::TexMetadata& metadata)
{
	mScratchImage = std::move(scratchImage);
	mMetadata = metadata;

	mType = TextureType::Default;

	mBufferResource = std::make_unique<BufferResource>();
	mUploadBuffer = std::make_unique<UploadBuffer>();

	// リソース設定
	D3D12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			mMetadata.format,
			static_cast<uint64_t>(mMetadata.width),
			static_cast<uint32_t>(mMetadata.height),
			static_cast<uint16_t>(mMetadata.arraySize),
			static_cast<uint16_t>(mMetadata.mipLevels),
			1);

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
		GetRequiredIntermediateSize(mBufferResource->buffer.Get(), 0, static_cast<uint16_t>(mMetadata.mipLevels));

	// アップロードバッファの生成
	mUploadBuffer->Create(uploadSize);
}

void Texture::UpLoad()
{
	// サブリソースデータを初期化
	std::vector<D3D12_SUBRESOURCE_DATA> subResourcesDatas(mMetadata.mipLevels);
	for (size_t i = 0; i < subResourcesDatas.size(); i++)
	{
		// イメージデータを取得してサブリソースデータを設定
		const DirectX::Image* img = mScratchImage.GetImage(i, 0, 0);
		subResourcesDatas[i].pData = img->pixels;
		subResourcesDatas[i].RowPitch = img->rowPitch;
		subResourcesDatas[i].SlicePitch = img->slicePitch;

	}
	// テクスチャーをアップロード
	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		mBufferResource->buffer.Get(),
		mUploadBuffer->GetBufferResource()->buffer.Get(),
		0,
		0,
		static_cast<uint32_t>(mMetadata.mipLevels),
		subResourcesDatas.data());

	// リソースの状態変更
	RenderBase::GetInstance()->TransitionBufferState(
		mBufferResource.get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ);
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
