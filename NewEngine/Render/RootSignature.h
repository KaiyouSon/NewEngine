#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>

class RootSignature
{
private:
	std::vector<D3D12_ROOT_PARAMETER> rootParameters;	// ルートパラメーターの設定
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//std::vector<CD3DX12_DESCRIPTOR_RANGE> descriptorRanges;
	HRESULT result;
	size_t constantBufferNum;
	size_t descriptorRangeNum;

public:
	void Create(size_t number);
	void AddConstantBufferViewToRootRrameter(const size_t& number);
	void AddDescriptorRangeToRootPrameter(size_t number);

	inline ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
	inline size_t GetConstantBufferNum() { return constantBufferNum; }
	inline UINT GetRootDescriptorTableIndex() { return (UINT)(rootParameters.size() - 1); }
};

