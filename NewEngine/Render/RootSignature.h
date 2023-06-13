#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>

class RootSignature
{
private:
	std::vector<D3D12_ROOT_PARAMETER> rootParameters_;	// ルートパラメーターの設定
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	//std::vector<CD3DX12_DESCRIPTOR_RANGE> descriptorRanges;
	HRESULT result_;
	uint32_t constantBufferNum_;
	uint32_t descriptorRangeNum_;

public:
	void Create(const uint32_t number);
	void AddConstantBufferViewToRootRrameter(const uint32_t number);
	void AddDescriptorRangeToRootPrameter(const uint32_t number);

	inline ID3D12RootSignature* GetRootSignature() { return rootSignature_.Get(); }
	inline uint32_t GetConstantBufferNum() { return constantBufferNum_; }
	inline uint32_t GetRootDescriptorTableIndex() { return (uint32_t)(rootParameters_.size() - 1); }
};

