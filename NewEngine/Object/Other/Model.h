#pragma once
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include <vector>
#include <string>

class FbxModel;

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

