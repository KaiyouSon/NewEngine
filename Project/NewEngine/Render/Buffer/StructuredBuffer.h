#pragma once
#include "BufferResource.h"
#include "RenderBase.h"
#include <cassert>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

class StructuredBuffer
{
private:
	std::unique_ptr<BufferResource> mBufferResource;

public:
	StructuredBuffer();
	void Create(const uint32_t dataSize);
	
public:
	BufferResource* GetBufferResource();
};

