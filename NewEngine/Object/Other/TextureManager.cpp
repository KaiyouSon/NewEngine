#include "TextureManager.h"
#include "RenderBase.h"
#include <DirectXTex.h>
using namespace DirectX;

std::map<std::string, std::unique_ptr<Texture>> TextureManager::textureMap;

Texture* TextureManager::GetTexture(const std::string& textureTag)
{
	return textureMap[textureTag].get();
}

Texture* TextureManager::CreateTexture(const Color& color, const std::string& textureTag)
{
	std::unique_ptr<Texture> tex;
	tex.reset(new Texture(color));
	textureMap.insert(std::make_pair(textureTag, std::move(tex)));

	return textureMap[textureTag].get();
}

Texture* TextureManager::LoadTexture(const std::string& filePath, const std::string& textureTag)
{
	std::unique_ptr<Texture> tex;
	tex.reset(new Texture(filePath));
	textureMap.insert(std::make_pair(textureTag, std::move(tex)));

	return textureMap[textureTag].get();
}