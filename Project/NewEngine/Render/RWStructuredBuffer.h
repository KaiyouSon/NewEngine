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
	T* mapping;	// �}�b�s���O�p

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

		// �q�[�v�̐ݒ�
		D3D12_HEAP_PROPERTIES heapProp =
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		// ���\�[�X�ݒ�
		D3D12_RESOURCE_DESC resourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer(
				(sizeof(T) + 0xff) & ~0xff,	// 256�o�C�g�A���C�������g
				D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		// �萔�o�b�t�@�̐���
		result = RenderBase::GetInstance()->GetDevice()->
			CreateCommittedResource(
				&heapProp,	// �q�[�v�̐ݒ�
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc, // ���\�[�X�̐ݒ�
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&mBufferResource->buffer));
		assert(SUCCEEDED(result));

		mBufferResource->buffer->SetName(L"RWStructuredBuffer");

		// �萔�o�b�t�@�̃}�b�s���O
		//result = mBufferResource->buffer->Map(0, nullptr, (void**)&mapping);	// �}�b�s���O
		//assert(SUCCEEDED(result));
	}

	BufferResource* GetBufferResource()
	{
		return mBufferResource.get();
	}

};

