#include "RootSignature.h"
#include "ShaderObject.h"
#include "RenderBase.h"
#include <d3dcompiler.h>
#include <cassert>

void RootSignature::Create()
{
	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;			// 一度の描画に使うテクスチャが1枚なので１
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;		// テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParam;
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // 種類
	rootParam.DescriptorTable.pDescriptorRanges = &descriptorRange;		  // デスクリプタレンジ
	rootParam.DescriptorTable.NumDescriptorRanges = 1;					  // デスクリプタレンジ数
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			  // 全てのシェーダから見える

	// テクスチャレジスタ0番（SRV）
	rootParameters.emplace_back(rootParam);

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
	rootSignatureDesc.pParameters = rootParameters.data();	// ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = rootParameters.size();		// ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	auto errorBlob = ShaderObject::GetErrorBlob();
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);
	assert(SUCCEEDED(result));

	result = RenderBase::GetInstance()->GetDevice()->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));

	assert(SUCCEEDED(result));
}

void RootSignature::AddConstantBufferToRootRrameter(const size_t& number)
{
	for (int i = 0; i < number; i++)
	{
		D3D12_ROOT_PARAMETER rootParam;
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 種類
		rootParam.Descriptor.ShaderRegister = rootParameters.size();	// 定数バッファ番号
		rootParam.Descriptor.RegisterSpace = 0;						// デフォルト値
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える

		rootParameters.emplace_back(rootParam);
	}
}
