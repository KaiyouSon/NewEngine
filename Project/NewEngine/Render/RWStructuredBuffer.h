#pragma once
#include "BufferResource.h"
#include "RenderBase.h"
#include "DescriptorHeapManager.h"
#include <cassert>

template<typename T>
class RWStructuredBuffer
{
private:
	std::unique_ptr<BufferResource> mBufferResource;

public:
	T* mapping;	// マッピング用

public:
	RWStructuredBuffer() : mapping(nullptr)
	{
		mBufferResource = nullptr;
	}
	~RWStructuredBuffer()
	{
		if (mBufferResource == nullptr)
		{
			return;
		}
		//mBufferResource->buffer->Unmap(0, nullptr);
	}

	void Create()
	{
		mBufferResource = std::make_unique<BufferResource>();

		HRESULT result;

		// ヒープの設定
		D3D12_HEAP_PROPERTIES heapProp =
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		// リソース設定
		D3D12_RESOURCE_DESC resourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer(
				(sizeof(T) + 0xff) & ~0xff,	// 256バイトアラインメント
				D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		// 定数バッファの生成
		result = RenderBase::GetInstance()->GetDevice()->
			CreateCommittedResource(
				&heapProp,	// ヒープの設定
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc, // リソースの設定
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&mBufferResource->buffer));
		assert(SUCCEEDED(result));

		mBufferResource->buffer->SetName(L"RWStructuredBuffer");

		// 定数バッファのマッピング
		//result = mBufferResource->buffer->Map(0, nullptr, (void**)&mapping);	// マッピング
		//assert(SUCCEEDED(result));
	}

	BufferResource* GetBufferResource()
	{
		return mBufferResource.get();
	}

};

