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
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescHeap;		// srv用デスクリプタヒープ

	static std::map<std::string, std::unique_ptr<Texture>> textureMap;
	static std::map<std::string, std::unique_ptr<RenderTexture>> renderTextureMap;

public:
	static Texture* GetTexture(std::string textureTag);
	static Texture CreateTexture(Color color);
	static Texture* CreateTexture(Color color, std::string textureTag);
	static Texture LoadTexture(std::string filePath);
	static Texture* LoadTexture(std::string filePath, std::string textureTag);
	static Texture LoadMaterialTexture(std::string filePath);

	static RenderTexture* GetRenderTexture(std::string textureTag);
	static RenderTexture* CreateRenderTexture(Vec2 size, size_t num, std::string textureTag);

	static void CreateDescriptorHeap();
	static void CreateSRV(Texture& texture, ID3D12Resource* buffer);
	static inline ID3D12DescriptorHeap* GetSrvDescHeap() { return srvDescHeap.Get(); }

	static void ExcuteComandList();
};

