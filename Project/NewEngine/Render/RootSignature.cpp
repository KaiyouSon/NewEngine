#include "RootSignature.h"
#include "ShaderObject.h"
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

	// RootParameterにConstantBufferViewを追加
	AddCbvToRootRrameter(mSetting.maxCbvRootParameter);

	// RootParameterにSRVのDescriptorRangeを追加
	AddSrvToRootPrameter(mSetting.maxSrvDescritorRange);

	// RootParameterにUAVのDescriptorRangeを追加
	AddUavToRootPrameter(mSetting.maxUavDescritorRange);

	// テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 横繰り返し(タイリング)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 縦繰り返し(タイリング)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 奥行繰り返し(タイリング)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					// 全てのリニア補間
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									// ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												// ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// ピクセルシェーダからのみ使用可能

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = mRootParameters.data();					// ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = (uint32_t)mRootParameters.size();		// ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	auto errorBlob = ShaderObject::GetErrorBlob();
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
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 種類
		rootParam.Descriptor.ShaderRegister = (uint32_t)mRootParameters.size();	// 定数バッファ番号
		rootParam.Descriptor.RegisterSpace = 0;						// デフォルト値
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える

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
	// 必ず0番目から作るようにするため0を返す
	return 0;
}
uint32_t RootSignature::GetSRVStartIndex()
{
	// maxCbvRootParameterがSRVの最初のIndexになる
	return mSetting.maxCbvRootParameter;
}
uint32_t RootSignature::GetUAVStartIndex()
{
	// maxSrvDescritorRangeがUAVの最初のIndexになる
	return
		mSetting.maxCbvRootParameter +
		mSetting.maxSrvDescritorRange;
}
