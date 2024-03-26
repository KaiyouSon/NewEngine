#pragma once
#include "BufferResource.h"
#include <memory>
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

// インデックスバッファのクラス
class IndexBuffer
{
private:
	std::unique_ptr<BufferResource> mBufferResource;
	D3D12_INDEX_BUFFER_VIEW mBufferView;

public:
	IndexBuffer();
	void Create(const std::vector<uint32_t>& indices);

public: // ゲッター
	D3D12_INDEX_BUFFER_VIEW* GetibViewAddress();
};
