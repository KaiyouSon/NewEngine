#include "Texture.h"

Texture::Texture() :
	mBufferResource(std::make_unique<BufferResource>()),
	uploadBuffer(nullptr), size(0)
{
}

// ÉQÉbÉ^Å[
BufferResource* Texture::GetBufferResource()
{
	return mBufferResource.get();
}
