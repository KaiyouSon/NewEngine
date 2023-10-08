#include "VolumeTexture.h"
#include "RenderBase.h"
#include <cassert>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

VolumeTexture::VolumeTexture() :
	mResult(HRESULT()), mUploadBuffer(std::make_unique<UploadBuffer>())
{
}

void VolumeTexture::Create(const Vec3 size)
{
	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// リソース設定
	D3D12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Tex3D(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			static_cast<uint64_t>(size.x),
			static_cast<uint32_t>(size.y),
			static_cast<uint16_t>(size.z),
			1);

	// バッファ生成
	mResult = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&mBufferResource->buffer));
	assert(SUCCEEDED(mResult));

	// 名前設定
	mBufferResource->buffer->SetName(L"VolumeTextureBuffer");
}
