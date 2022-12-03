#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "GameObject.h"
#include <vector>

class Line : public GameObject
{
private:
	VertexBuffer<VertexPosNormalUv>* vertexBuffer;
	IndexBuffer* indexBuffer;
	ConstantBuffer* constantBuffer;
	std::vector<VertexPosNormalUv> vertices;
	std::vector<unsigned short> indices;

public:
	Line();
	~Line();
	void Initialize(const Vec3& startPos, const Vec3& endPos);
	void Update();
	void Draw();
};

