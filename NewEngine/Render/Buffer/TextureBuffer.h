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
	static const size_t textureWidth = 256;	// �������s�N�Z����
	static const size_t textureHeight = 256;	// �c�����s�N�Z����
	static const size_t imageDataCount = textureWidth * textureHeight;	// �z��̗v�f��
	static Vec4* imageData;	// �摜�C���[�W�f�[�^�z��

public:
	~TextureBuffer();
	static std::unique_ptr<Texture> LoadTexture(const std::string filePath);
	static std::unique_ptr<Texture> GetDefaultTexture();
	static std::unique_ptr<Texture> GetRenderTexture(const Vec2& size);
	static void UnLoadTexture(Texture* texture);
};

