#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Texture.h"
#include <vector>

class Sprite
{
private:
	VertexBuffer<VertexPosUv>* vertexBuffer;
	std::vector<VertexPosUv> vertices;
	ConstantBuffer<ConstantBufferData::CTransform2D>* constantBufferTransform;
	ConstantBuffer<ConstantBufferData::CColor>* constantBufferColor;
	Transform transform;

public:
	Texture* texture;
	Vec2 pos;
	Vec2 scale;
	Vec2 uvPos;
	Vec2 size;
	float rot;
	Color color;
	Vec2 anchorPoint;
	GraphicsPipeline* graphicsPipeline;

private:
	void TransferTexturePos(const Vec2& size = 0);
	void SetBlendMode(const BlendMode& blendMode);

public:
	Sprite();
	~Sprite();
	void Update();
	void Draw(const BlendMode& blendMode = BlendMode::Alpha);

	void SetTextureRect(const Vec2& leftTopPos, const Vec2 rightDownPos);

};

