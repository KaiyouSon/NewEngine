#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"
#include "RenderTexture.h"
#include "Material.h"
#include <vector>

class PostEffect
{
private:
	std::vector<VertexPosUv> vertices_;
	std::unique_ptr<VertexBuffer<VertexPosUv>> vertexBuffer_;
	std::vector<RenderTexture*> renderTextures_;
	GraphicsPipeline* graphicsPipeline_;
	Material material_;
	Transform transform_;
	Vec2 size_;

public:
	Vec2 pos;
	Vec2 scale;
	Vec2 anchorPoint;
	float rot;
	Color color;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();
	void TransferTexturePos();

public:
	PostEffect();
	void Update();
	void Draw();

	void AddRenderTexture(RenderTexture* renderTexture);
	//void SetRenderTexture(RenderTexture* renderTexture);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
};

