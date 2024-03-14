#pragma once
#include "RenderBase.h"
#include "IConstantBuffer.h"
#include "ConstantBufferData.h"
#include <d3d12.h>
#include <cassert>
#include <wrl.h>
#include <memory>

// 定数バッファのクラス
template<typename T>
class ConstantBuffer : public IConstantBuffer
{
public:
	T* constantBufferMap; // 定数バッファのマップポインタ

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

		// ヒーププロパティ
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;    // GPUによって読み取り可能なヒープ
		// リソース記述
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(T) + 0xff) & ~0xff; // 256の倍数に切り上げることでパディング
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// コミットされたリソースの作成
		result = RenderBase::GetInstance()->GetDevice()->
			CreateCommittedResource(
				&cbHeapProp,    // ヒーププロパティ
				D3D12_HEAP_FLAG_NONE,
				&cbResourceDesc, // リソース記述
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&bufferResource->buffer));
		assert(SUCCEEDED(result));

		bufferResource->bufferState = D3D12_RESOURCE_STATE_GENERIC_READ;

		bufferResource->buffer->SetName(L"ConstantBuffer");

		// コミットされたリソースのマップ
		result = bufferResource->buffer->Map(0, nullptr, (void**)&constantBufferMap);    // マップ
		assert(SUCCEEDED(result));
	}
};

template<typename T>
void TransferDataToConstantBuffer(IConstantBuffer* iCBuffer, const T& data)
{
	// キャスト
	auto buffer = dynamic_cast<ConstantBuffer<T>*>(iCBuffer);

	if (buffer == nullptr)
	{
		return;
	}

	// マッピング
	*(buffer->constantBufferMap) = data;
}