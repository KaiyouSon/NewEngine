#pragma once
#include "Texture.h"
#include <list>
#include <string>

class TextureManager
{
private:
	std::list<Texture> textureList;

public:
	Texture GetTexture(const std::string& textureName);
};

