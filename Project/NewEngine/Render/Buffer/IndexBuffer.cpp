#include "IndexBuffer.h"
#include "RenderBase.h"
#include <cassert>

void IndexBuffer::Create(const std::vector<uint16_t>& indices)
{
	// 鬆らせ繝・・繧ｿ蜈ｨ菴薙・繧ｵ繧､繧ｺ
	uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint16_t) * indices.size());

	// 鬆らせ繝舌ャ繝輔ぃ縺ｮ險ｭ螳・
	D3D12_HEAP_PROPERTIES heapProp{}; // 繝偵・繝苓ｨｭ螳・
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU縺ｸ縺ｮ霆｢騾∫畑
	// 繝ｪ繧ｽ繝ｼ繧ｹ險ｭ螳・
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // 鬆らせ繝・・繧ｿ蜈ｨ菴薙・繧ｵ繧､繧ｺ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT result;

	// 繧､繝ｳ繝・ャ繧ｯ繧ｹ繝舌ャ繝輔ぃ縺ｮ逕滓・
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapProp, // 繝偵・繝苓ｨｭ螳・
			D3D12_HEAP_FLAG_NONE,
			&resDesc, // 繝ｪ繧ｽ繝ｼ繧ｹ險ｭ螳・
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mIndexBuffer));
	assert(SUCCEEDED(result));

	//------------------- 繧､繝ｳ繝・ャ繧ｯ繧ｹ繝舌ャ繝輔ぃ縺ｸ縺ｮ繝・・繧ｿ霆｢騾・-------------------//
	uint16_t* indexMap = nullptr;
	result = mIndexBuffer->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// 蜈ｨ鬆らせ縺ｫ蟇ｾ縺励※
	for (int i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i]; // 蠎ｧ讓吶ｒ繧ｳ繝斐・
	}
	// 郢九′繧翫ｒ隗｣髯､
	mIndexBuffer->Unmap(0, nullptr);

	// 繧､繝ｳ繝・ャ繧ｯ繧ｹ繝舌ャ繝輔ぃ繝薙Η繝ｼ縺ｮ菴懈・
	mIndexBufferView.BufferLocation = mIndexBuffer->GetGPUVirtualAddress();
	mIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
	mIndexBufferView.SizeInBytes = sizeIB;
}

D3D12_INDEX_BUFFER_VIEW* IndexBuffer::GetibViewAddress()
{
	return &mIndexBufferView;
}
