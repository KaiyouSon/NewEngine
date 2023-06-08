#pragma once
#include <d3d12.h>
#include <wrl.h>

class DepthBuffer
{
private:
	HRESULT result_;
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle_ = D3D12_CPU_DESCRIPTOR_HANDLE(); //RTVのハンドル(CPU側)

public:
	void Create();

public: // セッター
	inline void SetCpuHandle(const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) { this->cpuHandle_ = cpuHandle; }

public: // ゲッター
	inline ID3D12Resource* GetBuffer() { return buffer_.Get(); }
	inline ID3D12Resource** GetBufferAddress() { return buffer_.GetAddressOf(); }
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return cpuHandle_; }
};

