#include "DepthTexture.h"
#include "DescriptorHeapManager.h"

void DepthTexture::Create(DepthBuffer* depthBuffer)
{
	mType = TextureType::Depth;

	mBufferResource->buffer = depthBuffer->GetBufferResource()->buffer;
	mBufferResource->buffer->SetName(L"DepthTexture");
	mBufferResource->bufferState = depthBuffer->GetBufferResource()->bufferState;

	// SRV作成
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(mBufferResource.get());
}
