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

		// バッファの生成
		result = RenderBase::GetInstance()->GetDevice()->
			CreateCommittedResource(
				&heapProp,	// ヒープの設定
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc, // リソースの設定
				D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
				nullptr,
				IID_PPV_ARGS(&mBufferResource->buffer));
		assert(SUCCEEDED(result));

		mBufferResource->buffer->SetName(L"RWStructuredBuffer");
	}

	BufferResource* GetBufferResource()
	{
		return mBufferResource.get();
	}

};

