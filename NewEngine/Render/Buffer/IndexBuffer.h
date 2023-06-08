#pragma once
#include <vector>
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

class IndexBuffer
{
private:
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_;

public:
	void Create(std::vector<uint16_t> indices);

public:
	// インデックスバッファビューを取得する関数
	inline D3D12_INDEX_BUFFER_VIEW* GetibViewAddress() { return &ibView_; }
};

