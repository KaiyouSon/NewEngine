#include "DepthBuffer.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include <cassert>

void DepthBuffer::Create(const Vec2 size)
{
	mBufferResource = std::make_unique<BufferResource>();

	HRESULT result;

	Vec2 resourceSize = (size.x <= -1.f) ?
		RenderWindow::GetInstance()->GetWindowSize() : size;

	// リソース設定
	D3D12_RESOURCE_DESC depthResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			(uint32_t)resourceSize.x,
			(uint32_t)resourceSize.y,
			1, 1, 1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	// ヒープ設定
	D3D12_HEAP_PROPERTIES depthHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// クリア
	D3D12_CLEAR_VALUE clearValue =
		CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.f, 0);

	// バッファ生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&depthHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&depthResourceDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE, // 書き込み可能
			&clearValue,
			IID_PPV_ARGS(&mBufferResource->buffer));
	assert(SUCCEEDED(result));

	mBufferResource->bufferState = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	mBufferResource->buffer->SetName(L"DepthBuffer");
}

// ゲッター
BufferResource* DepthBuffer::GetBufferResource()
{
	return mBufferResource.get();
}
