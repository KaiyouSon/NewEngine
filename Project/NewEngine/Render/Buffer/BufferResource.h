#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

struct DescriptorHandle
{
	D3D12_CPU_DESCRIPTOR_HANDLE cpu; // CPU繝上Φ繝峨Ν
	D3D12_GPU_DESCRIPTOR_HANDLE gpu; // GPU繝上Φ繝峨Ν
};

struct BufferResource
{
	DescriptorHandle srvHandle;		// SRV逕ｨ
	DescriptorHandle rtvHandle;		// RTV逕ｨ
	DescriptorHandle dsvHandle;		// DSV逕ｨ
	DescriptorHandle uavHandle;		// UAV逕ｨ

	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
	uint32_t index;

	D3D12_RESOURCE_STATES bufferState;
};
