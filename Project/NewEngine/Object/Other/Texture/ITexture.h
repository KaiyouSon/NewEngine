#pragma once
#include "BufferResource.h"
#include "Vec3.h"
#include <memory>

// テクスチャーのインターフェース
class ITexture
{
protected:
	std::unique_ptr<BufferResource> mBufferResource;
	Vec3 mInitalSize;

public:
	ITexture();
	virtual ~ITexture() {}

public:
	// ゲッター
	BufferResource* GetBufferResource();
	Vec3 GetInitalSize();
};