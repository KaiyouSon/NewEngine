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
	std::vector<VertexPosUv> vertices;
	ConstantBuffer<ConstantBufferDataTransform2D>* constantBufferTransform;
	ConstantBuffer<ConstantBufferDataColor>* constantBufferColor;
	Transform transform;

public:
	Texture texture;
	Vec2 pos;
	Vec2 scale;
	float rot;
	Color color;
	Vec2 anchorPoint;

private:
	void TransferTexturePos();
	void TransferTextureUv();

public:
	Sprite();
	~Sprite();
	void Update();
	void Draw();

	inline Vec2 GetTextureSize() { return texture.size; }

	static void SetBlendMode(const BlendMode& blendMode);
};

