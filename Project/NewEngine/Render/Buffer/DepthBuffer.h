#pragma once
#include "BufferResource.h"
#include "Vec2.h"
#include <memory>

// 深度バッファのクラス
class DepthBuffer
{
private:
	std::unique_ptr<BufferResource> mBufferResource;

public:
	void Create(const Vec2 size = -1);

public: // ゲッター
	BufferResource* GetBufferResource();
};