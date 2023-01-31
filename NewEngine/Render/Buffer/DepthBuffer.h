#pragma once
#include <d3d12.h>
#include <wrl.h>

class DepthBuffer
{
private:
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE(); //RTVのハンドル(CPU側)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = D3D12_GPU_DESCRIPTOR_HANDLE(); //RTVのハンドル(GPU側)

public:
	void Create();

public: // セッター
	inline void SetCpuHandle(const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle) { this->cpuHandle = cpuHandle; }
	inline void SetGpuHandle(const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle) { this->gpuHandle = gpuHandle; }

public: // ゲッター
	inline ID3D12Resource* GetBuffer() { return buffer.Get(); }
	inline ID3D12Resource** GetBufferAddress() { return buffer.GetAddressOf(); }
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return cpuHandle; }
	inline D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() { return gpuHandle; }
};

