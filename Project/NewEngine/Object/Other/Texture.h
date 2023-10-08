#pragma once
#include "ITexture.h"
#include "UploadBuffer.h"
#include "Vec2.h"

class Texture : public ITexture
{
private:
	HRESULT mResult;

	std::unique_ptr<UploadBuffer> mUploadBuffer;
	Vec2 mInitalSize;	// 初期値のサイズ

public:
	// コンストラクタ
	Texture();

	// テクスチャーのバッファ生成する関数
	void Create(const D3D12_RESOURCE_DESC& resourceDesc, uint32_t mipLevels = 1);

public:
	// アップロードバッファの取得
	UploadBuffer* GetUploadBuffer();

	// 初期値のサイズ取得
	Vec2 GetInitalSize();
};
