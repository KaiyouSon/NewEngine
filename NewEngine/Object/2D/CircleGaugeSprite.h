#pragma once
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Model.h"
#include "Texture.h"
#include "Util.h"
#include "Sprite.h"

class CircleGaugeSprite
{
private:
	VertexBuffer<VertexPosUv>* vertexBuffer;
	std::vector<VertexPosUv> vertices;
	ConstantBuffer<ConstantBufferDataTransform2D>* constantBufferTransform;
	ConstantBuffer<ConstantBufferDataColor>* constantBufferColor;
	ConstantBuffer<ConstantBufferDataCircleGauge>* constantBufferCircleGauge;
	GraphicsPipeline* graphicsPipeline;
	Transform transform;

public:
	Texture texture;
	Vec2 pos;
	Vec2 scale;
	Vec2 uvPos;
	Vec2 size;
	float rot;
	Color color;
	Vec2 anchorPoint;
	float startRadian;
	float endRadian;

private:
	void TransferTexturePos();
	void SetBlendMode(const BlendMode& blendMode);

public:
	CircleGaugeSprite();
	~CircleGaugeSprite();
	void Update();
	void Draw(const BlendMode& blendMode = BlendMode::Alpha);
};
