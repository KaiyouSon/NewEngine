#pragma once
#include "ITexture.h"
#include "UploadBuffer.h"
#include "Vec3.h"

class VolumeTexture : public ITexture
{
private:
	HRESULT mResult;
	std::unique_ptr<UploadBuffer> mUploadBuffer;

public:
	// コンストラクタ
	VolumeTexture();

	// テクスチャーのバッファ生成する関数
	void Create(const Vec2 size);
};

