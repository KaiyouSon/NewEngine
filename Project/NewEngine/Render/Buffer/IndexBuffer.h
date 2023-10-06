#pragma once
#include <vector>
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

class IndexBuffer
{
private:
	// 繧､繝ｳ繝・ャ繧ｯ繧ｹ繝舌ャ繝輔ぃ繝薙Η繝ｼ
	D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
	Microsoft::WRL::ComPtr<ID3D12Resource> mIndexBuffer;

public:
	void Create(const std::vector<uint16_t>& indices);

public:
	// 繧､繝ｳ繝・ャ繧ｯ繧ｹ繝舌ャ繝輔ぃ繝薙Η繝ｼ繧貞叙蠕励☆繧矩未謨ｰ
	D3D12_INDEX_BUFFER_VIEW* GetibViewAddress();
};
