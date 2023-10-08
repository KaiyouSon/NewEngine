#pragma once
#include "BufferResource.h"
#include <memory>

// アップロードバッファのクラス
class UploadBuffer
{
private:
	HRESULT mResult;
	std::unique_ptr<BufferResource> mBufferResource;

public:
	UploadBuffer();

	// バッファを生成する関数
	void Create(const uint64_t uploadSize);

public:
	// ゲッター
	BufferResource* GetBufferResource();
};
