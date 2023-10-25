#pragma once
#include "ITexture.h"
#include "UploadBuffer.h"
#include "Vec2.h"
#include <DirectXTex.h>

class Texture : public ITexture
{
private:
	HRESULT mResult;

	DirectX::ScratchImage mScratchImage;
	std::unique_ptr<UploadBuffer> mUploadBuffer;

public:
	// テクスチャーのバッファ生成する関数
	void Create(const D3D12_RESOURCE_DESC& resourceDesc, uint32_t mipLevels = 1);

public:
	void SetScratchImage(DirectX::ScratchImage* scratchImage);

public:
	// アップロードバッファの取得
	UploadBuffer* GetUploadBuffer();

	// スクラッチイメージ
	DirectX::ScratchImage* GetScratchImage();
};
