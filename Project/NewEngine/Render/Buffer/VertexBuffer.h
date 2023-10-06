#pragma once
#include "VertexBufferData.h"
#include "RenderBase.h"
#include <cassert>
#include <vector>
#include <d3d12.h>
#include <wrl.h>

template<typename T> class VertexBuffer
{
private:
	HRESULT result;

	D3D12_VERTEX_BUFFER_VIEW vbView{};						// 鬆らせ繝舌ャ繝輔ぃ繝薙Η繝ｼ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;	// 鬆らせ繝舌ャ繝輔ぃ
	T* vertMap;

public:
	// 蛻晄悄蛹・
	void Create(const std::vector<T>& vertices)
	{
		// 鬆らせ繝・・繧ｿ蜈ｨ菴薙・繧ｵ繧､繧ｺ = 鬆らせ繝・・繧ｿ荳縺､蛻・・繧ｵ繧､繧ｺ * 鬆らせ繝・・繧ｿ縺ｮ隕∫ｴ謨ｰ
		unsigned int sizeVB = static_cast<unsigned int>(sizeof(T) * vertices.size());

		// 鬆らせ繝舌ャ繝輔ぃ縺ｮ險ｭ螳・
		D3D12_HEAP_PROPERTIES heapProp{}; // 繝偵・繝苓ｨｭ螳・
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU縺ｸ縺ｮ霆｢騾∫畑

		// 繝ｪ繧ｽ繝ｼ繧ｹ險ｭ螳・
		D3D12_RESOURCE_DESC resDesc{};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeVB; // 鬆らせ繝・・繧ｿ蜈ｨ菴薙・繧ｵ繧､繧ｺ
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 鬆らせ繝舌ャ繝輔ぃ縺ｮ逕滓・
		result = RenderBase::GetInstance()->GetDevice()->
			CreateCommittedResource(
				&heapProp, // 繝偵・繝苓ｨｭ螳・
				D3D12_HEAP_FLAG_NONE,
				&resDesc, // 繝ｪ繧ｽ繝ｼ繧ｹ險ｭ螳・
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&vertexBuffer));
		assert(SUCCEEDED(result));

		//----------------------- 鬆らせ繝舌ャ繝輔ぃ縺ｸ縺ｮ繝・・繧ｿ霆｢騾・-----------------------//
		// GPU荳翫・繝舌ャ繝輔ぃ縺ｫ蟇ｾ蠢懊＠縺滉ｻｮ諠ｳ繝｡繝｢繝ｪ(繝｡繧､繝ｳ繝｡繝｢繝ｪ荳・繧貞叙蠕・
		vertMap = nullptr;
		result = vertexBuffer->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));
		// 蜈ｨ鬆らせ縺ｫ蟇ｾ縺励※
		copy(vertices.begin(), vertices.end(), vertMap);

		// GPU莉ｮ諠ｳ繧｢繝峨Ξ繧ｹ
		vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		// 鬆らせ繝舌ャ繝輔ぃ縺ｮ繧ｵ繧､繧ｺ
		vbView.SizeInBytes = sizeVB;
		// 鬆らせ・代▽蛻・・繝・・繧ｿ繧ｵ繧､繧ｺ
		vbView.StrideInBytes = sizeof(vertices[0]);
	}

	// 霆｢騾・
	void TransferToBuffer(const std::vector<T>& vertices)
	{
		// 蜈ｨ鬆らせ縺ｫ蟇ｾ縺励※繧ｳ繝斐・
		copy(vertices.begin(), vertices.end(), vertMap);
	}

public:
	// 鬆らせ繝舌ャ繝輔ぃ繝薙Η繝ｼ縺ｮ繧｢繝峨Ξ繧ｹ繧貞叙蠕励☆繧矩未謨ｰ
	inline D3D12_VERTEX_BUFFER_VIEW* GetvbViewAddress() { return &vbView; }
	inline ID3D12Resource* GetVertexBuffer() { return vertexBuffer.Get(); }

};
