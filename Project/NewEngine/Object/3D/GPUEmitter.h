#pragma once
#include "Transform.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include "ComputePipeline.h"
#include "VertexBuffer.h"
#include "ParticleParam.h"
#include "StructuredBuffer.h"
#include "DescriptorHeapManager.h"

class GPUEmitter
{
private:
	std::vector<VertexBufferData::VParticle> mVertices;

	std::unique_ptr<StructuredBuffer> mParticleData;
	std::vector<std::unique_ptr<StructuredBuffer>> mStructuredBuffers;

	Vec3 mWorldPos;
	Vec3 mWorldScale;
	Transform mTransform;
	Transform* mParent;
	Texture* mTexture;
	Material mMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	ComputePipeline* mComputePipeline;
	Billboard mBillboard;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;
	Vec2 tiling;
	Vec2 offset;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	GPUEmitter();
	void Update(Transform* parent = nullptr);
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

public:
	// ComputeShaderのみで使うデータを増やす関数
	template<typename T>
	void AddStructuredBuffer()
	{
		// StructuredBufferの作成
		mStructuredBuffers.push_back(std::move(std::make_unique<StructuredBuffer>()));
		mStructuredBuffers.back()->Create(sizeof(T));

		// UAV作成
		DescriptorHeapManager::GetDescriptorHeap("SRV")->
			CreateUAV(mStructuredBuffers.back()->GetBufferResource(), 1, sizeof(T));
	}

public: //繧ｻ繝・ち繝ｼ

	// 繝・け繧ｹ繝√Ε繝ｼ
	void SetTexture(Texture* texture);

	// 繧ｰ繝ｩ繝輔ぅ繝・け繧ｹ繝代う繝励Λ繧､繝ｳ
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

	// Computeパイプラインを設定
	void SetComputePipeline(ComputePipeline* computePipeline);
	
	// パーティクルのデータ
	template<typename T>
	void SetParticleData(const uint32_t maxParticle)
	{
		mVertices.resize(maxParticle);

		// SRV縺ｨUAV繧剃ｽ懈・
		uint32_t dataSize = sizeof(T) * maxParticle;
		mParticleData->Create(dataSize);

		DescriptorHeapManager::GetDescriptorHeap("SRV")->
			CreateSRV(mParticleData->GetBufferResource(), maxParticle, sizeof(T));

		// GENERIC_READ -> UNORDERED_ACCESS 縺ｫ縺励※UAV繧剃ｽ懈・
		RenderBase::GetInstance()->TransitionBufferState(
			mParticleData->GetBufferResource(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		mParticleData->GetBufferResource()->buffer->GetDesc();

		DescriptorHeapManager::GetDescriptorHeap("SRV")->
			CreateUAV(mParticleData->GetBufferResource(), maxParticle, sizeof(T));
	}

public: // 繧ｲ繝・ち繝ｼ

	// 繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓・
	Vec3 GetWorldPos();

	// 繝ｯ繝ｼ繝ｫ繝峨せ繧ｱ繝ｼ繝ｫ
	Vec3 GetWorldScale();

	// 繝医Λ繝ｳ繧ｹ繝輔か繝ｼ繝
	Transform GetTransform();
};