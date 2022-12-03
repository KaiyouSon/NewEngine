#pragma once
#include "IComponent.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <vector>
#include <list>
#include <string>
#include <memory>

class Model : public IComponent
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

