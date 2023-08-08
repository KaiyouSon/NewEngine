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
	std::vector<VertexBufferData::VSprite> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VSprite>> mVertexBuffer;
	std::vector<RenderTexture*> mRenderTextures;
	GraphicsPipeline* mGraphicsPipeline;
	Material mMaterial;
	Transform mTransform;
	Vec2 mSize;
	Texture* mDepthTexture;

public:
	Vec2 pos;
	Vec2 scale;
	Vec2 anchorPoint;
	float rot;
	Color color;
	uint32_t rtvIndex;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();
	void TransferTexturePos();

public:
	PostEffect();
	void Update();
	void Draw();

public: // ÇªÇÃëº
	void AddRenderTexture(RenderTexture* renderTexture);

	template<typename T>
	void AddMaterial(const T& constantBuffer)
	{
		std::unique_ptr<IConstantBuffer> iConstatnBuffer = std::make_unique<T>();
		iConstatnBuffer->Create();
		mMaterial.constantBuffers.push_back(std::move(iConstatnBuffer));
	}

public: // ÉQÉbÉ^Å[
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
	void SetDrawCommands(const uint32_t registerNum, const uint32_t bufferNum);

	template<typename T>
	void SetTransferBuffer(const uint32_t bufferNum, const T& data)
	{
		uint32_t bNum = Min<uint32_t>(bufferNum, (uint32_t)mMaterial.constantBuffers.size());
		TransferDataToConstantBuffer(mMaterial.constantBuffers[bNum].get(), data);
	}
};

