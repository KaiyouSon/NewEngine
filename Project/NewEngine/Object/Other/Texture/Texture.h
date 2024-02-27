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
	DirectX::TexMetadata mMetadata;

	std::unique_ptr<UploadBuffer> mUploadBuffer;

public:
	Texture();
	Texture(const std::string tag, const std::string& path = std::string());

	// テクスチャーのバッファ生成する関数
	void Create(const D3D12_RESOURCE_DESC& resourceDesc, uint32_t mipLevels = 1);
	void Create(DirectX::ScratchImage& scratchImage, const DirectX::TexMetadata& metadata);
	void UpLoad();

public:
	void SetScratchImage(DirectX::ScratchImage* scratchImage);

public:
	// アップロードバッファの取得
	UploadBuffer* GetUploadBuffer();

	// スクラッチイメージ
	DirectX::ScratchImage* GetScratchImage();
};
