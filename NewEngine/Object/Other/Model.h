#pragma once
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include <vector>
#include <string>

class Model
{
public:
	Material material;
	Mesh mesh;

public:
	Model() {}
	Model(const std::string& modelName, const bool& isSmoothing = false);
	~Model() {};
};

