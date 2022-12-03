#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "GameObject.h"
#include <vector>

class Sprite : public GameObject
{
private:
	VertexBuffer<VertexPosUv>* vertexBuffer;
	IndexBuffer* indexBuffer;
	ConstantBuffer* constantBuffer;
	std::vector<VertexPosUv> vertices;
	std::vector<unsigned short> indices;
public:
	Sprite();
	~Sprite();
	void Initialize();
	void Update();
	void Draw();

	bool GetLayer();
	Mat4 GetFinalMat();
};

