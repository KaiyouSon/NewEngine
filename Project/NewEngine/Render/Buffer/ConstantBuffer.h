#pragma once
#include "RenderBase.h"
#include "IConstantBuffer.h"
#include "ConstantBufferData.h"
#include <d3d12.h>
#include <cassert>
#include <wrl.h>
#include <memory>

template<typename T>
class ConstantBuffer : public IConstantBuffer
{
public:
	T* constantBufferMap;	// マッピング用

public:
	ConstantBuffer() : constantBufferMap(nullptr)
	{
		bufferResource = nullptr;
	}
	~ConstantBuffer()
	{
		if (bufferResource == nullptr)
		{
			return;
		}
		bufferResource->buffer->Unmap(0, nullptr);
	}

	void Create() override
	{
		bufferResource = std::make_unique<BufferResource>();

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
				IID_PPV_ARGS(&bufferResource->buffer));
		assert(SUCCEEDED(result));

		//constantBuffer->SetName(L"ConstantBuffer");

		// 定数バッファのマッピング
		result = bufferResource->buffer->Map(0, nullptr, (void**)&constantBufferMap);	// マッピング
		assert(SUCCEEDED(result));
	}
};

template<typename T>
void TransferDataToConstantBuffer(IConstantBuffer* iCBuffer, const T& data)
{
	// キャスト
	auto buffer = dynamic_cast<ConstantBuffer<T>*>(iCBuffer);

	// 転送
	*(buffer->constantBufferMap) = data;
}