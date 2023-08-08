#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>

struct RootSignatureSetting
{
	uint32_t constantBufferViewNum;
	uint32_t descriptorRangeNum;
	RootSignatureSetting();
};

class RootSignature
{
private:
	std::vector<CD3DX12_DESCRIPTOR_RANGE> mDescriptorRanges;
	std::vector<CD3DX12_ROOT_PARAMETER> mRootParameters;	// ルートパラメーターの設定
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;
	HRESULT mResult;
	uint32_t mConstantBufferNum;
	uint32_t mDescriptorRangeNum;


private:
	void AddConstantBufferViewToRootRrameter(const uint32_t number);
	void AddDescriptorRangeToRootPrameter(const uint32_t number);

public:
	void Create(const RootSignatureSetting setting);

	ID3D12RootSignature* GetRootSignature();
	uint32_t GetConstantBufferStartIndex();
	uint32_t GetConstantBufferEndIndex();
	uint32_t GetDescriptorTableStartIndex();
	uint32_t GetDescriptorTableEndIndex();
};

