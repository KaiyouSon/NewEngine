#pragma once
#include "Texture.h"
#include <string>
#include <map>

class TextureManager
{
private:
	static std::map<std::string, Texture> textureMap;

public:
	static Texture GetTexture(const std::string& textureTag);
	static Texture LoadTexture(const std::string& filePath, const std::string textureTag);
};

