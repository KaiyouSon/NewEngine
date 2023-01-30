#pragma once
#include "MathUtil.h"
#include <d3d12.h>
#include <wrl.h>

class RenderTarget
{
private:
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE(); //RTVのハンドル(CPU側)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = D3D12_GPU_DESCRIPTOR_HANDLE(); //RTVのハンドル(GPU側)
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer; // レンダーターゲットのリソース

public: // セッター
	inline void SetCpuHandle(const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle) { this->cpuHandle = cpuHandle; }
	inline void SetGpuHandle(const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle) { this->gpuHandle = gpuHandle; }

public: // ゲッター
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return cpuHandle; }
	inline D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() { return gpuHandle; }
};

