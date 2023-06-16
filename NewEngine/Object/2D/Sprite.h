#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Texture.h"
#include "Material.h"
#include "TextureAnimeiton.h"
#include <vector>

class TextureAnimeiton;

class Sprite
{
private:
	std::vector<VertexPosUv> vertices_;
	std::unique_ptr<VertexBuffer<VertexPosUv>> vertexBuffer_;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CTransform2D>> constantBufferTransform_;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CColor>> constantBufferColor_;
	Transform transform_;
	Transform* parent_;
	Texture* texture_;
	Material* material_;

public:
	Vec2 pos;
	Vec2 scale;
	Vec2 size;
	float rot;
	Color color;
	Vec2 anchorPoint;
	GraphicsPipeline* graphicsPipeline;

private:
	void TransferTexturePos(const Vec2& size = 0);
	void SetBlendMode(const BlendMode blendMode);

public:
	Sprite();
	void Update(Transform* parent = nullptr);
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

	void SetTextureRect(const Vec2 leftTopPos, const Vec2 rightDownPos);
	void SetTexture(Texture* texture);

private:
	friend TextureAnimeiton;
};

