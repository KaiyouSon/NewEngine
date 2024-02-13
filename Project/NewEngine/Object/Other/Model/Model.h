#pragma once
#include "MaterialColor.h"
#include "TextureManager.h"
#include "Mesh.h"
#include "NewEngineEnum.h"
#include <string>

// モデルのインターフェース
struct Model
{
	std::string name;
	std::string tag;
	Mesh mesh;
	MaterialColor material;
	ModelFormat format;
	Texture* texture = TextureManager::GetTexture("White");

	virtual ~Model() {}
};
