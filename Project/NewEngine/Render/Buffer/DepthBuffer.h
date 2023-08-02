#pragma once
#include <d3d12.h>
#include <wrl.h>

class DepthBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> mBuffer;
	D3D12_CPU_DESCRIPTOR_HANDLE mCpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE(); //RTVのハンドル(CPU側)

public:
	void Create();

public: // セッター
	void SetCpuHandle(const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle);

public: // ゲッター
	ID3D12Resource* GetBuffer();
	ID3D12Resource** GetBufferAddress();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle();
};

