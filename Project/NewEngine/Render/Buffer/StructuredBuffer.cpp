#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer() : mBufferResource(nullptr)
{
}

BufferResource* StructuredBuffer::GetBufferResource()
{
	return mBufferResource.get();
}