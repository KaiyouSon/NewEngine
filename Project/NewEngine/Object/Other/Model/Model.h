#pragma once
#include "MaterialColor.h"
#include "TextureManager.h"
#include "Mesh.h"
#include "NewEngineEnum.h"
#include <string>

// モデルの基底構造体
struct Model
{
	std::string name;
	Mesh mesh;
	MaterialColor material;
	ModelFormat format;
	Texture* texture = TextureManager::GetTexture("White");

	virtual ~Model() {}
};
