#include "Texture.h"

Texture::Texture() :
	mBufferResource(std::make_unique<BufferResource>()),
	uploadBuffer(nullptr), size(0)
{
}

// �Q�b�^�[
BufferResource* Texture::GetBufferResource()
{
	return mBufferResource.get();
}
