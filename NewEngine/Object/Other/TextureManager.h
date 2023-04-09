#pragma once
#include "Texture.h"
#include "TextureAnimeiton.h"
#include "RenderTexture.h"
#include <DirectXTex.h>
#include <string>
#include <map>

class TextureManager
{
private:
	static UINT srvIncrementIndex;
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescHeap;		// srv�p�f�X�N���v�^�q�[�v

	static std::map<std::string, std::unique_ptr<Texture>> textureMap;
	static std::map<std::string, std::unique_ptr<RenderTexture>> renderTextureMap;

public:
	static Texture* GetTexture(const std::string& textureTag);
	static Texture CreateTexture(const Color& color);
	static Texture* CreateTexture(const Color& color, const std::string& textureTag);
	static Texture LoadTexture(const std::string& filePath);
	static Texture* LoadTexture(const std::string& filePath, const std::string& textureTag);
	static Texture LoadMaterialTexture(const std::string& filePath);

	static RenderTexture* GetRenderTexture(const std::string& textureTag);
	static RenderTexture* CreateRenderTexture(const Vec2& size, const size_t num, const std::string& textureTag);

	static void CreateDescriptorHeap();
	static void CreateSRV(Texture& texture, ID3D12Resource* buffer);
	static inline ID3D12DescriptorHeap* GetSrvDescHeap() { return srvDescHeap.Get(); }

	static void ExcuteComandList();
};

