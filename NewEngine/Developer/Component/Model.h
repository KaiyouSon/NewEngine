#pragma once
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <vector>
#include <string>

class Model
{
public:
	VertexBuffer<VertexPosNormalUv> vertexBuffer;
	IndexBuffer indexBuffer;

	std::vector<VertexPosNormalUv> vertices;
	std::vector<unsigned short> indices;

public:
	Model() {}
	Model(std::string modelName);
	~Model() {};
};

