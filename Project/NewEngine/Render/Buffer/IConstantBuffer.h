#pragma once
#include "BufferResource.h"
#include <memory>

// 定数バッファのインターフェース
class IConstantBuffer
{
public:
	std::unique_ptr<BufferResource> bufferResource;

public:
	virtual void Create() = 0;
	virtual ~IConstantBuffer() {}
};
