#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>

class RootSignature
{
private:
	std::vector<D3D12_ROOT_PARAMETER> mRootParameters;	// ルートパラメーターの設定
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;
	HRESULT mResult;
	uint32_t mConstantBufferNum;
	uint32_t mDescriptorRangeNum;

public:
	void Create(const uint32_t number);
	void AddConstantBufferViewToRootRrameter(const uint32_t number);
	void AddDescriptorRangeToRootPrameter(const uint32_t number);

	ID3D12RootSignature* GetRootSignature();
	uint32_t GetConstantBufferNum();
	uint32_t GetRootDescriptorTableIndex();
};

