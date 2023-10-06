#pragma once
#include "BufferResource.h"
#include <memory>

class RenderTarget
{
private:
	std::unique_ptr<BufferResource> mBufferResource;

public:
	RenderTarget();

public:
	// 繧ｲ繝・ち繝ｼ
	BufferResource* GetBufferResource();

};

