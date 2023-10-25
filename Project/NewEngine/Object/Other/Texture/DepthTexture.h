#pragma once
#include "ITexture.h"
#include "DepthBuffer.h"

// 深度テクスチャのクラス
class DepthTexture : public ITexture
{
public:
	// テクスチャーのバッファ生成する関数
	void Create(DepthBuffer* depthBuffer);
};