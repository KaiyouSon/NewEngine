#pragma once
#include <d3d12.h>
#include <wrl.h>

class DepthBuffer
{
private:
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE(); //RTV�̃n���h��(CPU��)

public:
	void Create();

public: // �Z�b�^�[
	inline void SetCpuHandle(const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle) { this->cpuHandle = cpuHandle; }

public: // �Q�b�^�[
	inline ID3D12Resource* GetBuffer() { return buffer.Get(); }
	inline ID3D12Resource** GetBufferAddress() { return buffer.GetAddressOf(); }
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return cpuHandle; }
};

