#pragma once
#include "MathUtil.h"
#include <d3d12.h>
#include <wrl.h>

class RenderTarget
{
private:
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE(); //RTV�̃n���h��(CPU��)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = D3D12_GPU_DESCRIPTOR_HANDLE(); //RTV�̃n���h��(GPU��)
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer; // �����_�[�^�[�Q�b�g�̃��\�[�X

public: // �Z�b�^�[
	inline void SetCpuHandle(const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle) { this->cpuHandle = cpuHandle; }
	inline void SetGpuHandle(const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle) { this->gpuHandle = gpuHandle; }

public: // �Q�b�^�[
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return cpuHandle; }
	inline D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() { return gpuHandle; }
};

