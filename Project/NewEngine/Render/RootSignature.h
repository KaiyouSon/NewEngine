#pragma once
#include "NewEngineDefine.h"
#include "NewEngineSetting.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

class RootSignature
{
private:
	HRESULT mResult;
	std::vector<CD3DX12_DESCRIPTOR_RANGE> mSrvDescriptorRanges;
	std::vector<CD3DX12_DESCRIPTOR_RANGE> mUavDescriptorRanges;
	std::vector<CD3DX12_ROOT_PARAMETER> mRootParameters;	// 繝ｫ繝ｼ繝医ヱ繝ｩ繝｡繝ｼ繧ｿ繝ｼ縺ｮ險ｭ螳・
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;
	RootSignatureSetting mSetting;

private:
	void AddCbvToRootRrameter(const uint32_t maxCbvRootParameter);
	void AddSrvToRootPrameter(const uint32_t maxSrvDescritorRange);
	void AddUavToRootPrameter(const uint32_t maxUavDescritorRange);

public:
	void Create(const RootSignatureSetting setting);

	ID3D12RootSignature* GetRootSignature();

	uint32_t GetCBVStartIndex();
	uint32_t GetSRVStartIndex();
	uint32_t GetUAVStartIndex();
};
