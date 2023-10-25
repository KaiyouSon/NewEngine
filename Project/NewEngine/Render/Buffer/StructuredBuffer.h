#pragma once
#include "BufferResource.h"
#include <memory>

// ストラクチャーバッファのクラス
class StructuredBuffer
{
private:
	std::unique_ptr<BufferResource> mBufferResource;

public:
	StructuredBuffer();
	void Create(const uint32_t dataSize);

public: // ゲッター
	BufferResource* GetBufferResource();
};