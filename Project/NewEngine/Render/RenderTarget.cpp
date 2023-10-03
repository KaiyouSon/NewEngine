#include "RenderTarget.h"

RenderTarget::RenderTarget() :
	mBufferResource(std::make_unique<BufferResource>())
{
}

BufferResource* RenderTarget::GetBufferResource()
{
	return mBufferResource.get();
}
