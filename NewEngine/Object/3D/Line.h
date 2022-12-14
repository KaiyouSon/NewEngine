#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <vector>

class Line
{
private:
	VertexBuffer<VertexPosNormalUv>* vertexBuffer;
	IndexBuffer* indexBuffer;
	std::vector<VertexPosNormalUv> vertices;
	std::vector<unsigned short> indices;
	ConstantBuffer<ConstantBufferDataTransform3D>* constantBufferTransform;
	ConstantBuffer<ConstantBufferDataColor>* constantBufferColor;

public:
	Line();
	~Line();
	void Initialize(const Vec3& startPos, const Vec3& endPos);
	void Update();
	void Draw();
};

