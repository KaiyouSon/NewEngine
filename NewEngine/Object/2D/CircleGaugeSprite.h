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
	std::vector<VertexBufferData::VSprite> vertices_;
	std::unique_ptr<VertexBuffer<VertexBufferData::VSprite>> vertexBuffer_;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CTransform2D>> constantBufferTransform_;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CColor>> constantBufferColor_;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CCircleGauge>> constantBufferCircleGauge_;
	GraphicsPipeline* graphicsPipeline_;
	Transform transform_;

public:
	Texture* texture;
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
	void SetBlendMode(const BlendMode blendMode);

public:
	CircleGaugeSprite();
	void Update();
	void Draw(const BlendMode blendMode = BlendMode::Alpha);
};
