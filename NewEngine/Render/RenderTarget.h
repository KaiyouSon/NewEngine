#pragma once
#include <d3d12.h>
#include <wrl.h>

class RenderTarget
{
private:
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE(); //RTV�̃n���h��(CPU��)

public: // �Z�b�^�[
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer; // �����_�[�^�[�Q�b�g�̃��\�[�X
	inline void SetCpuHandle(const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle) { this->cpuHandle = cpuHandle; }

public: // �Q�b�^�[
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return cpuHandle; }
	inline ID3D12Resource* GetBuffer() { return buffer.Get(); }
	inline ID3D12Resource** GetBufferAddress() { return buffer.GetAddressOf(); }

};

