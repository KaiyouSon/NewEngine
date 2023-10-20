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
	std::unique_ptr<Material> mMaterial;
	std::unique_ptr<Material> mCSMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	ComputePipeline* mComputePipeline;
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
	void CSMaterialTransfer();
	void CSMaterialDrawCommands();
	void TransferTexturePos();
	void TransferVertexCoord();

public:
	PostEffect();
	void Update();
	void ExecuteCS(const uint32_t threadX = 1, const uint32_t threadY = 1, const uint32_t threadZ = 1);
	void Draw();

public: // レンダーテクスチャ追加
	void AddRenderTexture(RenderTexture* renderTexture);

	// マテリアル追加
	template<typename T>
	void AddMaterial()
	{
		std::unique_ptr<IConstantBuffer> iConstatnBuffer = std::make_unique<ConstantBuffer<T>>();
		iConstatnBuffer->Create();
		mMaterial->constantBuffers.push_back(std::move(iConstatnBuffer));
	}

	// SBをマテリアルに追加（UAVとしてCS使うSRVとして他のShaderで使う）
	template<typename T>
	void AddSBToMaterial(const uint32_t arraySize = 1)
	{
		// StructuredBufferの作成
		mCSMaterial->structuredBuffers.push_back(std::move(std::make_unique<StructuredBuffer>()));
		mCSMaterial->structuredBuffers.back()->Create(sizeof(T) * arraySize);

		// SRV作成
		DescriptorHeapManager::GetDescriptorHeap("SRV")->
			CreateSRV(mCSMaterial->structuredBuffers.back()->GetBufferResource(), arraySize, sizeof(T));

		// GENERIC_READ -> UNORDERED_ACCESS に変更
		RenderBase::GetInstance()->TransitionBufferState(
			mCSMaterial->structuredBuffers.back()->GetBufferResource(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

		// UAV作成
		DescriptorHeapManager::GetDescriptorHeap("SRV")->
			CreateUAV(mCSMaterial->structuredBuffers.back()->GetBufferResource(), arraySize, sizeof(T));
	}

public: // セッター
	void SetSize(const Vec2 size);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
	void SetComputePipeline(ComputePipeline* computePipeline);

	template<typename T>
	void SetTransferBuffer(const uint32_t bufferNum, const T& data)
	{
		uint32_t bNum = Min<uint32_t>(bufferNum, (uint32_t)mMaterial->constantBuffers.size());
		TransferDataToConstantBuffer(mMaterial->constantBuffers[bNum].get(), data);
	}
};
