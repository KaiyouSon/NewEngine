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

TextureType ITexture::GetTextureType()
{
	return mType;
}

std::string ITexture::GetTag()
{
	return mTag;
}

std::string ITexture::GetPath()
{
	return mPath.c_str();
}
