#pragma once
#include "BufferResource.h"
#include "MathUtil.h"
#include "Color.h"
#include <string>
#include <memory>

class Texture
{
private:
	std::unique_ptr<BufferResource> mBufferResource;

public:
	Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;
	Vec2 size = 0;
	bool isMaterial = false;

public:
	Texture();

public: // ゲッター
	BufferResource* GetBufferResource();

};
