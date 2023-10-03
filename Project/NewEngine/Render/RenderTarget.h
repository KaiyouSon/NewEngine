#pragma once
#include "BufferResource.h"
#include <memory>

class RenderTarget
{
private:
	std::unique_ptr<BufferResource> mBufferResource;

public:
	RenderTarget();

public:
	// �Q�b�^�[
	BufferResource* GetBufferResource();

};

