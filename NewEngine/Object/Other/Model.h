#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "MaterialColor.h"
#include "TextureManager.h"
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
	std::string name;
	Mesh mesh;
	MaterialColor material;
	ModelFormat format;
	Texture* texture;

	virtual ~Model() {}
};

struct ObjModel : public Model
{

	ObjModel()
	{
		format = ModelFormat::Obj;
		texture = TextureManager::GetTexture("White");
	}
};

struct FbxModel : public Model
{
	std::vector<Bone> bones;

	FbxModel()
	{
		format = ModelFormat::Fbx;
		texture = TextureManager::GetTexture("White");
	}
};