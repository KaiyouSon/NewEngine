#pragma once
#include "ITexture.h"
#include "DepthBuffer.h"

class DepthTexture : public ITexture
{
private:
	HRESULT mResult;

public:
	// コンストラクタ
	DepthTexture();

	// テクスチャーのバッファ生成する関数
	void Create(DepthBuffer* depthBuffer);
};

