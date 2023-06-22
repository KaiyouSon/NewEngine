#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "MaterialColor.h"
#include "Mesh.h"
#include "Mat4.h"
#include "Enum.h"
#include <vector>
#include <string>

class FbxModel1;

struct Bone
{
	std::string name;
	Mat4 invInitalPose;
};

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
	std::vector<Bone> bones;

	FbxModel()
	{
		format = ModelFormat::Fbx;
	}
};