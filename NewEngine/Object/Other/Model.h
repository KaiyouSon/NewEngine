#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "MaterialColor.h"
#include "Mesh.h"
#include "Enum.h"
#include <vector>
#include <string>

class FbxModel1;

struct Model
{
	Mesh mesh;
	MaterialColor material;
	ModelFormat format;
	Texture texture;

	virtual ~Model() {}
};

struct ObjModel : public Model
{
	std::string name;

	ObjModel()
	{
		format = ModelFormat::Obj;
	}
};

struct FbxModel : public Model
{
	std::string name;

	FbxModel()
	{
		format = ModelFormat::Fbx;
	}
};