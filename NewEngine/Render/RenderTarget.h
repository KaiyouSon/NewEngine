#pragma once
#include <d3d12.h>
#include <wrl.h>

class RenderTarget
{
private:
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE(); //RTV�̃n���h��(CPU��)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = D3D12_GPU_DESCRIPTOR_HANDLE(); //RTV�̃n���h��(GPU��)

public: // �Z�b�^�[
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer; // �����_�[�^�[�Q�b�g�̃��\�[�X
	inline void SetCpuHandle(const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle) { this->cpuHandle = cpuHandle; }
	inline void SetGpuHandle(const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle) { this->gpuHandle = gpuHandle; }
	//inline void SetBuffer(const ID3D12Resource* buffer) { this->buffer = buffer; }

public: // �Q�b�^�[
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return cpuHandle; }
	inline D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() { return gpuHandle; }
	inline ID3D12Resource* GetBuffer() { return buffer.Get(); }
	inline ID3D12Resource** GetBufferAddress() { return buffer.GetAddressOf(); }

};

