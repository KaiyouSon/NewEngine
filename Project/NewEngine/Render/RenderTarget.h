#pragma once
#include <d3d12.h>
#include <wrl.h>

class RenderTarget
{
private:
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle_ = D3D12_CPU_DESCRIPTOR_HANDLE(); //RTV�̃n���h��(CPU��)

public: // �Z�b�^�[
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer_; // �����_�[�^�[�Q�b�g�̃��\�[�X
	inline void SetCpuHandle(const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle) { this->cpuHandle_ = cpuHandle; }

public: // �Q�b�^�[
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return cpuHandle_; }
	inline ID3D12Resource* GetBuffer() { return buffer_.Get(); }
	inline ID3D12Resource** GetBufferAddress() { return buffer_.GetAddressOf(); }

};

