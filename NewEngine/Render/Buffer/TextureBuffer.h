#pragma once
#include "Texture.h"
#include "MathUtil.h"
#include <d3d12.h>
#include <string>
#include <memory>
#include <wrl.h>
#include <DirectXTex.h>

class TextureBuffer
{
private:
	static const size_t textureWidth = 256;	// 横方向ピクセル数
	static const size_t textureHeight = 256;	// 縦方向ピクセル数
	static const size_t imageDataCount = textureWidth * textureHeight;	// 配列の要素数
	static Vec4* imageData;	// 画像イメージデータ配列

public:
	~TextureBuffer();
	static std::unique_ptr<Texture> LoadTexture(const std::string filePath);
	static std::unique_ptr<Texture> GetDefaultTexture();
	static std::unique_ptr<Texture> GetRenderTexture(const Vec2& size);
	static void UnLoadTexture(Texture* texture);
};

