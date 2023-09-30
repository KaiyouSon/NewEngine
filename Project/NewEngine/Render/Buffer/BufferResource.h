#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

struct BufferResource
{
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle; // CPU�n���h��
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle; // GPU�n���h��
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
	uint32_t index;
};

