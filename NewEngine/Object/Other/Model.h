#pragma once
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include <vector>
#include <string>

class FbxModel1;

class Model
{
public:
	Material material;
	Mesh mesh;
	std::string modelType = "OBJ";
public:
	Model() {}
	Model(const std::string modelName, const bool isSmoothing = false);
	virtual ~Model() {}
};

struct FbxModel : public Model
{
	std::string name;
	//Material material;
	//Mesh mesh;

	//std::vector<FbxModelNode> nodes;
	//std::vector<FbxModelBone> bones;
	//FbxModelNode* meshNode = nullptr;
	//FbxScene* fbxScene = nullptr;
	//FbxAnimetion fbxAnimetion;
};