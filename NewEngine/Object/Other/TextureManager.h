#pragma once
#include "Texture.h"
#include <string>
#include <map>

class TextureManager
{
private:
	static std::map<std::string, std::unique_ptr<Texture>> textureMap;

public:
	static Texture* GetTexture(const std::string& textureTag);
	static Texture* CreateTexture(const Color& color, const std::string& textureTag);
	static Texture* LoadTexture(const std::string& filePath, const std::string& textureTag);
};

