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
	T* constantBufferMap;	// 繝槭ャ繝斐Φ繧ｰ逕ｨ

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

		// 繝偵・繝励・險ｭ螳・
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPU縺ｸ縺ｮ霆｢騾∫畑
		// 繝ｪ繧ｽ繝ｼ繧ｹ險ｭ螳・
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(T) + 0xff) & ~0xff; // 256繝舌う繝医い繝ｩ繧､繝ｳ繝｡繝ｳ繝・
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 螳壽焚繝舌ャ繝輔ぃ縺ｮ逕滓・
		result = RenderBase::GetInstance()->GetDevice()->
			CreateCommittedResource(
				&cbHeapProp,	// 繝偵・繝励・險ｭ螳・
				D3D12_HEAP_FLAG_NONE,
				&cbResourceDesc, // 繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ險ｭ螳・
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&bufferResource->buffer));
		assert(SUCCEEDED(result));

		bufferResource->bufferState = D3D12_RESOURCE_STATE_GENERIC_READ;

		bufferResource->buffer->SetName(L"ConstantBuffer");

		// 螳壽焚繝舌ャ繝輔ぃ縺ｮ繝槭ャ繝斐Φ繧ｰ
		result = bufferResource->buffer->Map(0, nullptr, (void**)&constantBufferMap);	// 繝槭ャ繝斐Φ繧ｰ
		assert(SUCCEEDED(result));
	}
};

template<typename T>
void TransferDataToConstantBuffer(IConstantBuffer* iCBuffer, const T& data)
{
	// 繧ｭ繝｣繧ｹ繝・
	auto buffer = dynamic_cast<ConstantBuffer<T>*>(iCBuffer);

	if (buffer == nullptr)
	{
		return;
	}

	// 霆｢騾・
	*(buffer->constantBufferMap) = data;
}
