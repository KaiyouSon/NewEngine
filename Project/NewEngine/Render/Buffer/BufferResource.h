#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

struct BufferResource
{
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle; // CPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle; // GPUハンドル
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
	uint32_t index;
};

