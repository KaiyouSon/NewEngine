#pragma once
#include "BufferResource.h"
#include <memory>

class IConstantBuffer
{
public:
	std::unique_ptr<BufferResource> bufferResource;

public:
	virtual void Create() = 0;
	virtual ~IConstantBuffer() {}
};