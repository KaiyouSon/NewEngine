#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

struct DescriptorHandle
{
	D3D12_CPU_DESCRIPTOR_HANDLE cpu; // CPU�n���h��
	D3D12_GPU_DESCRIPTOR_HANDLE gpu; // GPU�n���h��
};

struct BufferResource
{
	DescriptorHandle srvHandle;		// SRV�p
	DescriptorHandle rtvHandle;		// RTV�p
	DescriptorHandle dsvHandle;		// DSV�p
	DescriptorHandle uavHandle;		// UAV�p

	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
	uint32_t index;

	D3D12_RESOURCE_STATES bufferState;
};