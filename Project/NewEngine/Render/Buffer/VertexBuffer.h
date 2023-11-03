#pragma once
#include "VertexBufferData.h"
#include "BufferResource.h"
#include "RenderBase.h"
#include <cassert>
#include <vector>
#include <d3d12.h>
#include <wrl.h>

// 頂点バッファのクラス
template<typename T> class VertexBuffer
{
private:
	HRESULT result;

	std::unique_ptr<BufferResource> mBufferResource;
	D3D12_VERTEX_BUFFER_VIEW mBufferView{};
	T* vertMap;

public:
	// コンストラクタ
	VertexBuffer() :mBufferResource(nullptr), vertMap(nullptr)
	{
	}

	// 頂点バッファの作成
	void Create(const std::vector<T>& vertices)
	{
		mBufferResource = std::make_unique<BufferResource>();

		// 頂点データサイズ = 1頂点のサイズ * 頂点数
		uint32_t sizeVB = static_cast<uint32_t>(sizeof(T) * vertices.size());

		// 頂点バッファのプロパティ
		D3D12_HEAP_PROPERTIES heapProp{};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUにアップロードするためのヒープ

		// リソースの説明
		D3D12_RESOURCE_DESC resDesc{};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeVB; // 頂点データサイズ
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 頂点バッファの作成
		result = RenderBase::GetInstance()->GetDevice()->
			CreateCommittedResource(
				&heapProp, // ヒーププロパティ
				D3D12_HEAP_FLAG_NONE,
				&resDesc, // リソースの説明
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&mBufferResource->buffer));
		assert(SUCCEEDED(result));

		//------------------- 頂点バッファのマップ -------------------//
		// GPUメモリに頂点バッファをマップ
		result = mBufferResource->buffer->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));

		// 頂点データをコピー
		std::memcpy(vertMap, vertices.data(), sizeVB);

		// GPUメモリのアンマップ
		mBufferResource->buffer->Unmap(0, nullptr);

		// 頂点バッファビューの設定
		mBufferView.BufferLocation = mBufferResource->buffer->GetGPUVirtualAddress();
		mBufferView.SizeInBytes = sizeVB;
		mBufferView.StrideInBytes = sizeof(vertices[0]);
	}

	// 頂点データをバッファに転送
	void TransferToBuffer(const std::vector<T>& vertices)
	{
		// GPUメモリに頂点バッファをマップ
		result = mBufferResource->buffer->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));

		// 頂点データをコピー
		std::memcpy(vertMap, vertices.data(), sizeof(T) * vertices.size());

		// GPUメモリのアンマップ
		mBufferResource->buffer->Unmap(0, nullptr);
	}

public:
	// 頂点バッファビューを取得
	D3D12_VERTEX_BUFFER_VIEW* GetvbViewAddress() { return &mBufferView; }
	// 頂点バッファを取得
	ID3D12Resource* GetVertexBuffer() { return mBufferResource->buffer.Get(); }

	BufferResource* GetBufferResource() { return mBufferResource.get(); }
};