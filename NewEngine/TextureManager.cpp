#include "TextureManager.h"

std::map<std::string, Texture> TextureManager::textureMap;

Texture TextureManager::GetTexture(const std::string& textureTag)
{
	return textureMap[textureTag];
}

Texture TextureManager::LoadTexture(const std::string& filePath, const std::string textureTag)
{
	Texture tex = Texture(filePath);
	textureMap.insert(std::make_pair(textureTag, tex));

	return tex;
}
