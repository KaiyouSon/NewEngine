#include "ITexture.h"

ITexture::ITexture() :
	mBufferResource(std::make_unique<BufferResource>())
{
}

BufferResource* ITexture::GetBufferResource()
{
	return mBufferResource.get();
}

Vec3 ITexture::GetInitalSize()
{
	return mInitalSize;
}
