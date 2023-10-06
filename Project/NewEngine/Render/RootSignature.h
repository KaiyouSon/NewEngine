#pragma once
#include "NewEngineDefine.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

struct RootSignatureSetting
{
	uint32_t maxCbvRootParameter;
	uint32_t maxSrvDescritorRange;
	uint32_t maxUavDescritorRange;
	RootSignatureSetting();
};

class RootSignature
{
private:
	HRESULT mResult;
	std::vector<CD3DX12_DESCRIPTOR_RANGE> mSrvDescriptorRanges;
	std::vector<CD3DX12_DESCRIPTOR_RANGE> mUavDescriptorRanges;
	std::vector<CD3DX12_ROOT_PARAMETER> mRootParameters;	// ルートパラメーターの設定
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

