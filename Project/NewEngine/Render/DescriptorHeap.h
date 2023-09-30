#pragma once
#include "BufferResource.h"
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>
#include <vector>

struct DescriptorHeapSetting
{
	enum HeapType
	{
		CBV_SRV_UAV,
		RTV,
		DSV,
	};

	// �T�C�Y
	uint32_t maxSize;

	// �q�[�v�̎��
	HeapType heapType;
};

class DescriptorHeap
{
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescriptorHeap;		// Srv��Uav�p�f�X�N���v�^�q�[�v(ComputShader�p)
	std::vector<bool> mCheckIndex;
	DescriptorHeapSetting mSetting;
	HRESULT mResult;

private:
	uint32_t GetIncrementIndex();
	uint32_t GetIncrementSize();

public:
	void Create(const DescriptorHeapSetting setting);

	void CreateSRV(BufferResource* bufferResource, const uint32_t arraySize = 0, const uint32_t byteSize = 0);
	void CreateUAV(BufferResource* bufferResource, const uint32_t arraySize = 0, const uint32_t byteSize = 0);

public:
	ID3D12DescriptorHeap* GetDescriptorHeap();

};
