#include "RootSignature.h"
#include "ShaderCompiler.h"
#include "RenderBase.h"
#include <d3dcompiler.h>
#include <cassert>

RootSignatureSetting::RootSignatureSetting() :
	maxCbvRootParameter(1), maxSrvDescritorRange(1), maxUavDescritorRange(0)
{
}


void RootSignature::Create(const RootSignatureSetting setting)
{
	mSetting = setting;

	// RootParameter縺ｫConstantBufferView繧定ｿｽ蜉
	AddCbvToRootRrameter(mSetting.maxCbvRootParameter);

	// RootParameter縺ｫSRV縺ｮDescriptorRange繧定ｿｽ蜉
	AddSrvToRootPrameter(mSetting.maxSrvDescritorRange);

	// RootParameter縺ｫUAV縺ｮDescriptorRange繧定ｿｽ蜉
	AddUavToRootPrameter(mSetting.maxUavDescritorRange);

	// 繝・け繧ｹ繝√Ε繧ｵ繝ｳ繝励Λ繝ｼ縺ｮ險ｭ螳・
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 讓ｪ郢ｰ繧願ｿ斐＠(繧ｿ繧､繝ｪ繝ｳ繧ｰ)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 邵ｦ郢ｰ繧願ｿ斐＠(繧ｿ繧､繝ｪ繝ｳ繧ｰ)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 螂･陦檎ｹｰ繧願ｿ斐＠(繧ｿ繧､繝ｪ繝ｳ繧ｰ)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// 繝懊・繝繝ｼ縺ｮ譎ゅ・鮟・
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					// 蜈ｨ縺ｦ縺ｮ繝ｪ繝九い陬憺俣
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									// 繝溘ャ繝励・繝・・譛螟ｧ蛟､
	samplerDesc.MinLOD = 0.0f;												// 繝溘ャ繝励・繝・・譛蟆丞､
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// 繝斐け繧ｻ繝ｫ繧ｷ繧ｧ繝ｼ繝縺九ｉ縺ｮ縺ｿ菴ｿ逕ｨ蜿ｯ閭ｽ

	// 繝ｫ繝ｼ繝医す繧ｰ繝阪メ繝｣縺ｮ險ｭ螳・
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = mRootParameters.data();					// 繝ｫ繝ｼ繝医ヱ繝ｩ繝｡繝ｼ繧ｿ縺ｮ蜈磯ｭ繧｢繝峨Ξ繧ｹ
	rootSignatureDesc.NumParameters = (uint32_t)mRootParameters.size();		// 繝ｫ繝ｼ繝医ヱ繝ｩ繝｡繝ｼ繧ｿ謨ｰ
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// 繝ｫ繝ｼ繝医す繧ｰ繝阪メ繝｣縺ｮ繧ｷ繝ｪ繧｢繝ｩ繧､繧ｺ
	auto errorBlob = ShaderCompiler::GetErrorBlob();
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
	mResult = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);
	assert(SUCCEEDED(mResult));

	mResult = RenderBase::GetInstance()->GetDevice()->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&mRootSignature));

	assert(SUCCEEDED(mResult));
}

void RootSignature::AddCbvToRootRrameter(const uint32_t maxCbvRootParameter)
{
	for (uint32_t i = 0; i < maxCbvRootParameter; i++)
	{
		D3D12_ROOT_PARAMETER rootParam;
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 遞ｮ鬘・
		rootParam.Descriptor.ShaderRegister = (uint32_t)mRootParameters.size();	// 螳壽焚繝舌ャ繝輔ぃ逡ｪ蜿ｷ
		rootParam.Descriptor.RegisterSpace = 0;						// 繝・ヵ繧ｩ繝ｫ繝亥､
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 蜈ｨ縺ｦ縺ｮ繧ｷ繧ｧ繝ｼ繝縺九ｉ隕九∴繧・

		mRootParameters.emplace_back(rootParam);
	}
}
void RootSignature::AddSrvToRootPrameter(const uint32_t maxSrvDescritorRange)
{
	for (uint32_t i = 0; i < maxSrvDescritorRange; i++)
	{
		mSrvDescriptorRanges.emplace_back();
		mSrvDescriptorRanges.back().Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, (uint32_t)i);
	}

	for (uint32_t i = 0; i < maxSrvDescritorRange; i++)
	{
		mRootParameters.emplace_back();
		mRootParameters.back().InitAsDescriptorTable(1, &mSrvDescriptorRanges[i]);
	}
}

void RootSignature::AddUavToRootPrameter(const uint32_t maxUavDescritorRange)
{
	for (uint32_t i = 0; i < maxUavDescritorRange; i++)
	{
		mUavDescriptorRanges.emplace_back();
		mUavDescriptorRanges.back().Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, (uint32_t)i);
	}

	for (uint32_t i = 0; i < maxUavDescritorRange; i++)
	{
		mRootParameters.emplace_back();
		mRootParameters.back().InitAsDescriptorTable(1, &mUavDescriptorRanges[i]);
	}
}

ID3D12RootSignature* RootSignature::GetRootSignature()
{
	return mRootSignature.Get();
}
uint32_t RootSignature::GetCBVStartIndex()
{
	// 蠢・★0逡ｪ逶ｮ縺九ｉ菴懊ｋ繧医≧縺ｫ縺吶ｋ縺溘ａ0繧定ｿ斐☆
	return 0;
}
uint32_t RootSignature::GetSRVStartIndex()
{
	// maxCbvRootParameter縺郡RV縺ｮ譛蛻昴・Index縺ｫ縺ｪ繧・
	return mSetting.maxCbvRootParameter;
}
uint32_t RootSignature::GetUAVStartIndex()
{
	// maxSrvDescritorRange縺袈AV縺ｮ譛蛻昴・Index縺ｫ縺ｪ繧・
	return
		mSetting.maxCbvRootParameter +
		mSetting.maxSrvDescritorRange;
}

