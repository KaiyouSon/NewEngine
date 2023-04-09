#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <vector>

class Line
{
private:
	std::vector<VertexPos> vertices;
	VertexBuffer<VertexPos>* vertexBuffer;
	ConstantBuffer<ConstantBufferDataTransform3D>* constantBufferTransform;
	ConstantBuffer<ConstantBufferDataColor>* constantBufferColor;
	GraphicsPipeline* graphicsPipeline;
	Transform transform;

public:
	Vec3 pos;
	float scale;
	Vec3 rot;
	Color color;

public:
	Line();
	~Line();
	void Update();
	void Draw();
};

