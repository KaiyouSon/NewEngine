#pragma once
#include "BufferResource.h"
#include "Vec2.h"
#include <memory>

class DepthBuffer
{
private:
	std::unique_ptr<BufferResource> mBufferResource;

public:
	void Create(const Vec2 size = -1);

public: // 繧ｲ繝・ち繝ｼ
	BufferResource* GetBufferResource();
};
