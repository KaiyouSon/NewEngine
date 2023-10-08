#pragma once
#include "BufferResource.h"
#include <memory>

// テクスチャーのインターフェース
class ITexture
{
protected:
	std::unique_ptr<BufferResource> mBufferResource;

public:
	ITexture();
	BufferResource* GetBufferResource();
};