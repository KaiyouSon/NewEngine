#include "DepthTexture.h"

DepthTexture::DepthTexture() : mResult(HRESULT())
{
}

void DepthTexture::Create(DepthBuffer* depthBuffer)
{
	mBufferResource->buffer = depthBuffer->GetBufferResource()->buffer;
	mBufferResource->buffer->SetName(L"DepthTextureBuffer");
}
