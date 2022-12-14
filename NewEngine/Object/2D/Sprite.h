#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Texture.h"
#include "Util.h"
#include <vector>

class Sprite
{
private:
	VertexBuffer<VertexPosUv>* vertexBuffer;
	IndexBuffer* indexBuffer;
	std::vector<VertexPosUv> vertices;
	std::vector<unsigned short> indices;
	ConstantBuffer<ConstantBufferDataTransform2D>* constantBufferTransform;
	ConstantBuffer<ConstantBufferDataColor>* constantBufferColor;
	Transform transform;

public:
	Texture texture;
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;
	Vec2 anchorPoint;

public:
	Sprite();
	~Sprite();
	void Update();
	void Draw();

	static void SetBlendMode(const BlendMode& blendMode);
};

