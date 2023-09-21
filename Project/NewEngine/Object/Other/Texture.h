#pragma once
#include "MathUtil.h"
#include "Color.h"
#include <d3d12.h>
#include <wrl.h>
#include <string>

class Texture
{
private:
	D3D12_CPU_DESCRIPTOR_HANDLE mCpuHandle; //SRVのハンドル(CPU側)
	D3D12_GPU_DESCRIPTOR_HANDLE mGpuHandle; //SRVのハンドル(GPU側)
	uint32_t mSrvIndex;

public:
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer; //テクスチャのリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;
	Vec2 size = 0;
	bool isMaterial = false;

public:
	Texture();

public: // セッター
	void SetCpuHandle(const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle);
	void SetGpuHandle(const D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle);
	void SetSRVIndex(const uint32_t srvIndex);

public: // ゲッター
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle();
	uint32_t GetSRVIndex();

};