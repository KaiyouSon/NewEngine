#pragma once
#include "Transform.h"
#include "TextureManager.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include "ComputePipeline.h"
#include "VertexBuffer.h"
#include "ParticleParam.h"
#include "StructuredBuffer.h"
#include "DescriptorHeapManager.h"

class ParticleMesh
{
private:
	std::unique_ptr<StructuredBuffer> mParticleData;
	std::vector<std::unique_ptr<StructuredBuffer>> mStructuredBuffers;

	uint32_t mMaxParticle;
	Texture* mMeshTexture;
	Texture* mParticleTexture;

	Vec3 mWorldPos;
	Vec3 mWorldScale;
	Transform mTransform;
	Transform* mParent;
	Material mMaterial;
	std::unique_ptr<Material> mCSMaterial;
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

	void CSMaterialInit();
	void CSMaterialTransfer();
	void CSMaterialDrawCommands();

public:
	ParticleMesh();
	void ExecuteComputeShader();
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

public: // セッター

	// メッシュのテクスチャー	
	void SetMeshTexture(Texture* meshTexture);

	// パーティクルのテクスチャー
	void SetParticleTexture(Texture* texture);

	// Graphicsパイプラインを設定
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

	// Computeパイプラインを設定
	void SetComputePipeline(ComputePipeline* computePipeline);

	// パーティクルのデータ
	template<typename T>
	void SetParticleData()
	{
		// SRV縺ｨUAV繧剃ｽ懈・
		uint32_t dataSize = sizeof(T) * mMaxParticle;
		mParticleData->Create(dataSize);

		DescriptorHeapManager::GetDescriptorHeap("SRV")->
			CreateSRV(mParticleData->GetBufferResource(), mMaxParticle, sizeof(T));

		// GENERIC_READ -> UNORDERED_ACCESS 縺ｫ縺励※UAV繧剃ｽ懈・
		RenderBase::GetInstance()->TransitionBufferState(
			mParticleData->GetBufferResource(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		mParticleData->GetBufferResource()->buffer->GetDesc();

		DescriptorHeapManager::GetDescriptorHeap("SRV")->
			CreateUAV(mParticleData->GetBufferResource(), mMaxParticle, sizeof(T));
	}

public: // ゲッター

	// 繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓・
	Vec3 GetWorldPos();

	// 繝ｯ繝ｼ繝ｫ繝峨せ繧ｱ繝ｼ繝ｫ
	Vec3 GetWorldScale();

	// 繝医Λ繝ｳ繧ｹ繝輔か繝ｼ繝
	Transform GetTransform();
};
