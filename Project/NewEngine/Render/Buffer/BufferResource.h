#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>
#include <vector>
#include "NewEngineEnum.h"

struct DescriptorHandle
{
	D3D12_CPU_DESCRIPTOR_HANDLE cpu; // CPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE gpu; // GPUハンドル
};

struct ViewIndex
{
	uint32_t index;
	ViewType viewType;
};

// バッファのクラス
struct BufferResource
{
	DescriptorHandle srvHandle;		// SRV用
	DescriptorHandle rtvHandle;		// RTV用
	DescriptorHandle dsvHandle;		// DSV用
	DescriptorHandle uavHandle;		// UAV用

	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
	std::vector<ViewIndex> viewIndexes;

	D3D12_RESOURCE_STATES bufferState;
};
