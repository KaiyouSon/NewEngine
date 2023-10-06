#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

struct DescriptorHandle
{
	D3D12_CPU_DESCRIPTOR_HANDLE cpu; // CPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE gpu; // GPUハンドル
};

struct BufferResource
{
	DescriptorHandle srvHandle;		// SRV用
	DescriptorHandle rtvHandle;		// RTV用
	DescriptorHandle dsvHandle;		// DSV用
	DescriptorHandle uavHandle;		// UAV用

	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
	uint32_t index;

	D3D12_RESOURCE_STATES bufferState;
};