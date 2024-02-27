#pragma once
#include "MaterialColor.h"
#include "TextureManager.h"
#include "Mesh.h"
#include "NewEngineEnum.h"
#include <string>

// モデルのインターフェース
class Model
{
protected:
	std::string mPath;

public:
	std::string name;
	std::string tag;
	Mesh mesh;
	MaterialColor material;
	ModelFormat format;
	Texture* texture;

public:
	Model();
	virtual ~Model() {}
};
