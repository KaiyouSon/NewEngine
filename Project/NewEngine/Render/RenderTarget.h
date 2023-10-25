#pragma once
#include "BufferResource.h"
#include <memory>

// レンダーターゲットのクラス
class RenderTarget
{
private:
	std::unique_ptr<BufferResource> mBufferResource;

public:
	RenderTarget();

public: // ゲッター
	BufferResource* GetBufferResource();
};