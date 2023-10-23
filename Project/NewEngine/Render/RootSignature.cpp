#include "RootSignature.h"
#include "ShaderCompiler.h"
#include "RenderBase.h"
#include <d3dcompiler.h>
#include <cassert>

void RootSignature::Create(const RootSignatureSetting setting)
{
    mSetting = setting;

    // RootParameterにConstantBufferView(CBV)を追加
    AddCbvToRootRrameter(mSetting.maxCbvRootParameter);

    // RootParameterにShaderResourceView(SRV)のDescriptorRangeを追加
    AddSrvToRootPrameter(mSetting.maxSrvDescritorRange);

    // RootParameterにUnorderedAccessView(UAV)のDescriptorRangeを追加
    AddUavToRootPrameter(mSetting.maxUavDescritorRange);

    // 静的サンプラー設定
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;  // テクスチャアドレッシングモード（U軸）
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;  // テクスチャアドレッシングモード（V軸）
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;  // テクスチャアドレッシングモード（W軸）
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;  // ボーダーカラー
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;  // テクスチャフィルタリング
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;  // レベルオブディテールの最大値
    samplerDesc.MinLOD = 0.0f;  // レベルオブディテールの最小値
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;  // シェーダ可視性（ピクセルシェーダ）

    // ルートシグネチャ記述
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pParameters = mRootParameters.data();  // ルートパラメータのデータ
    rootSignatureDesc.NumParameters = (uint32_t)mRootParameters.size();  // ルートパラメータの数
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;

    // ルートシグネチャのシリアライズ
    auto errorBlob = ShaderCompiler::GetErrorBlob();
    Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
    mResult = D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1_0,
        &rootSigBlob,
        &errorBlob);
    assert(SUCCEEDED(mResult));

    // デバイスにルートシグネチャを作成
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
        rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // パラメータのタイプ（CBV）
        rootParam.Descriptor.ShaderRegister = (uint32_t)mRootParameters.size();  // シェーダレジスタ番号（CBVの場合はルートパラメータの数）
        rootParam.Descriptor.RegisterSpace = 0;  // レジスタ番号
        rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;  // シェーダ可視性（全てのシェーダで可視）

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
	// CBVの開始インデックスは常に0で、変更不要
	return 0;
}
uint32_t RootSignature::GetSRVStartIndex()
{
	// 最大CBVルートパラメータの後にSRVのインデックスが続く
	return mSetting.maxCbvRootParameter;
}
uint32_t RootSignature::GetUAVStartIndex()
{
	// 最大SRVデスクリプタ範囲の後にUAVのインデックスが続く
	return
		mSetting.maxCbvRootParameter +
		mSetting.maxSrvDescritorRange;
}
