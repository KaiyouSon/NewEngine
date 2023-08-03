#pragma once
#include <d3d12.h>
#include <wrl.h>

class DepthBuffer
{
private:
	D3D12_CPU_DESCRIPTOR_HANDLE mCpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE(); //RTV�̃n���h��(CPU��)

public:
	Microsoft::WRL::ComPtr<ID3D12Resource> mBuffer;
	void Create();

public: // �Z�b�^�[
	void SetCpuHandle(const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle);

public: // �Q�b�^�[
	ID3D12Resource* GetBuffer();
	ID3D12Resource** GetBufferAddress();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle();
};

