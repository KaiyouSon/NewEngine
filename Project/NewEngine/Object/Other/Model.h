#pragma once
#include "MaterialColor.h"
#include "TextureManager.h"
#include "Mesh.h"
#include "Enum.h"
#include <string>

struct Model
{
	std::string name;
	Mesh mesh;
	MaterialColor material;
	ModelFormat format;
	Texture* texture = TextureManager::GetTexture("White");

	virtual ~Model() {}
};
