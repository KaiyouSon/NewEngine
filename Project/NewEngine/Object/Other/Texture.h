#pragma once
#include "MathUtil.h"
#include "Color.h"
#include <d3d12.h>
#include <wrl.h>
#include <string>

class Texture
{
private:
	D3D12_CPU_DESCRIPTOR_HANDLE mCpuHandle; //SRV�̃n���h��(CPU��)
	D3D12_GPU_DESCRIPTOR_HANDLE mGpuHandle; //SRV�̃n���h��(GPU��)
	uint32_t mSrvIndex;

public:
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer; //�e�N�X�`���̃��\�[�X
	Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;
	Vec2 size = 0;
	bool isMaterial = false;

public:
	Texture();

public: // �Z�b�^�[
	void SetCpuHandle(const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle);
	void SetGpuHandle(const D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle);
	void SetSRVIndex(const uint32_t srvIndex);

public: // �Q�b�^�[
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle();
	uint32_t GetSRVIndex();

};