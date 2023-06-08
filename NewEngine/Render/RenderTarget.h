#pragma once
#include <d3d12.h>
#include <wrl.h>

class RenderTarget
{
private:
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle_ = D3D12_CPU_DESCRIPTOR_HANDLE(); //RTVのハンドル(CPU側)

public: // セッター
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer_; // レンダーターゲットのリソース
	inline void SetCpuHandle(const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle) { this->cpuHandle_ = cpuHandle; }

public: // ゲッター
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return cpuHandle_; }
	inline ID3D12Resource* GetBuffer() { return buffer_.Get(); }
	inline ID3D12Resource** GetBufferAddress() { return buffer_.GetAddressOf(); }

};

