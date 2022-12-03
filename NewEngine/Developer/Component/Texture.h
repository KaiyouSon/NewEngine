#pragma once
#include "MathUtil.h"
#include <d3d12.h>
#include <wrl.h>
#include <string>

class Texture
{
private:
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE(); //SRV�̃n���h��(CPU��)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = D3D12_GPU_DESCRIPTOR_HANDLE(); //SRV�̃n���h��(GPU��)
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer; //�e�N�X�`���̃��\�[�X
	Vec2 size;

public:
	Texture() {};
	Texture(std::string filePath);

public: // �Z�b�^�[
	inline void SetCpuHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) { this->cpuHandle = cpuHandle; }
	inline void SetGpuHandle(D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle) { this->gpuHandle = gpuHandle; }

public: // �Q�b�^�[
	inline D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() { return gpuHandle; }
};