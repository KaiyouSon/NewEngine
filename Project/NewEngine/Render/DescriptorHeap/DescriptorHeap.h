#pragma once
#include "BufferResource.h"
#include "NewEngineSetting.h"
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>
#include <vector>

// ディスクリプタヒープのクラス
class DescriptorHeap
{
private:
	// ディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescriptorHeap;
	std::vector<bool> mCheckIndex;	// 何番目使ってるかどうかを判定するよう
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
