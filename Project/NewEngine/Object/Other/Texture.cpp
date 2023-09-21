#include "Texture.h"

Texture::Texture() :
	mCpuHandle(D3D12_CPU_DESCRIPTOR_HANDLE()),
	mGpuHandle(D3D12_GPU_DESCRIPTOR_HANDLE()),
	buffer(nullptr), uploadBuffer(nullptr),
	mSrvIndex(0), size(0)
{
}

void Texture::SetSRVIndex(const uint32_t srvIndex)
{
	mSrvIndex = srvIndex;
}

uint32_t Texture::GetSRVIndex()
{
	return mSrvIndex;
}

// セッター
void Texture::SetCpuHandle(const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle)
{
	mCpuHandle = cpuHandle;
}
void Texture::SetGpuHandle(const D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle)
{
	mGpuHandle = gpuHandle;
}

// ゲッター
D3D12_GPU_DESCRIPTOR_HANDLE Texture::GetGpuHandle()
{
	return mGpuHandle;
}