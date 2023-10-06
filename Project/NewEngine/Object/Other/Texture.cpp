#include "Texture.h"

Texture::Texture() :
	mBufferResource(std::make_unique<BufferResource>()),
	uploadBuffer(nullptr), size(0)
{
}

// ゲッター
BufferResource* Texture::GetBufferResource()
{
	return mBufferResource.get();
}
