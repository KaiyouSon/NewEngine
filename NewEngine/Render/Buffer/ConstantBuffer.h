#pragma once
#include "RenderBase.h"
#include "ConstantBufferData.h"
#include <d3d12.h>
#include <wrl.h>

template<typename T>
class ConstantBuffer
{
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer;//	定数バッファ
	T* constantBufferMap;	// マッピング用

public:
	~ConstantBuffer()
	{
		constantBuffer->Unmap(0, nullptr);
	}

	void Init()
	{
		HRESULT result;

		// ヒープの設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPUへの転送用
		// リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(T) + 0xff) & ~0xff; // 256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 定数バッファの生成
		result = RenderBase::GetInstance()->GetDevice()->
			CreateCommittedResource(
				&cbHeapProp,	// ヒープの設定
				D3D12_HEAP_FLAG_NONE,
				&cbResourceDesc, // リソースの設定
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&constantBuffer));
		assert(SUCCEEDED(result));

		constantBuffer->SetName(L"ConstantBuffer");

		// 定数バッファのマッピング
		result = constantBuffer->Map(0, nullptr, (void**)&constantBufferMap);	// マッピング
		assert(SUCCEEDED(result));
	}
};