#pragma once
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "Enum.h"
#include <vector>
#include <string>

class FbxModel1;

struct Model
{
public:
	Mesh mesh;
	MaterialColor material;
	ModelFormat format;

	std::string modelType = "OBJ";
public:
	//Model(const std::string modelName, const bool isSmoothing = false);
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

	//Material material;
	//Mesh mesh;

	//std::vector<FbxModelNode> nodes;
	//std::vector<FbxModelBone> bones;
	//FbxModelNode* meshNode = nullptr;
	//FbxScene* fbxScene = nullptr;
	//FbxAnimetion fbxAnimetion;
};