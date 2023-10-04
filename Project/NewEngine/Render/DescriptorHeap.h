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
		None,
		CBV_SRV_UAV,
		RTV,
		DSV,
	};

	// サイズ
	uint32_t maxSize;

	uint32_t startIndex;

	// ヒープの種類
	HeapType heapType;

	DescriptorHeapSetting();
};

class DescriptorHeap
{
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescriptorHeap;		// SrvとUav用デスクリプタヒープ(ComputShader用)
	std::vector<bool> mCheckIndex;
	DescriptorHeapSetting mSetting;
	HRESULT mResult;

private:
	uint32_t GetIncrementIndex();
	uint32_t GetIncrementSize();

public:
	void Create(const DescriptorHeapSetting setting);

	void CreateRTV(BufferResource* bufferResource);
	void CreateDSV(BufferResource* bufferResource);
	void CreateSRV(BufferResource* bufferResource, const uint32_t arraySize = 0, const uint32_t byteSize = 0);
	void CreateUAV(BufferResource* bufferResource, const uint32_t arraySize = 0, const uint32_t byteSize = 0);

	void DestroyView(BufferResource* bufferResource);

public:
	ID3D12DescriptorHeap* GetDescriptorHeap();

};

