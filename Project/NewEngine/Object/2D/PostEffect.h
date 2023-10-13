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
	Texture* mDepthTexture;
	Vec2 mAnchorPoint;
	Vec2 mSize;

public:
	Vec2 pos;
	Vec2 scale;
	float rot;
	Color color;
	uint32_t rtvIndex;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();
	void TransferTexturePos();
	void TransferVertexCoord();

public:
	PostEffect();
	void Update();
	void Draw();

public: // 縺昴・莉・
	void AddRenderTexture(RenderTexture* renderTexture);

	template<typename T>
	void AddMaterial()
	{
		std::unique_ptr<IConstantBuffer> iConstatnBuffer = std::make_unique<T>();
		iConstatnBuffer->Create();
		mMaterial.constantBuffers.push_back(std::move(iConstatnBuffer));
	}

public: // 繧ｲ繝・ち繝ｼ
	void SetSize(const Vec2 size);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
	void SetDrawCommands(const uint32_t registerNum, const uint32_t bufferNum);

	template<typename T>
	void SetTransferBuffer(const uint32_t bufferNum, const T& data)
	{
		uint32_t bNum = Min<uint32_t>(bufferNum, (uint32_t)mMaterial.constantBuffers.size());
		TransferDataToConstantBuffer(mMaterial.constantBuffers[bNum].get(), data);
	}
};
