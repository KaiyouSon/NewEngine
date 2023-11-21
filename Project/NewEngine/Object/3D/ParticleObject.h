#pragma once
#include "Transform.h"
#include "TextureManager.h"
#include "Model.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include "ComputePipeline.h"
#include "VertexBuffer.h"
#include "ParticleParam.h"
#include "StructuredBuffer.h"
#include "DescriptorHeapManager.h"

// GPUパーティクルを用いて、メッシュの貼り付けるクラス
class ParticleObject
{
private:
	std::unique_ptr<StructuredBuffer> mParticleData;

	Model* mModel;
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
	BlendMode mBlendMode;

private:
	// パーティクル関連
	uint32_t mMeshParticleSize;
	uint32_t mMaxParticleSize;

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

	void DrawCommands();

public:
	ParticleObject();
	~ParticleObject();
	void ExecuteCS(const uint32_t threadX = 1, const uint32_t threadY = 1, const uint32_t threadZ = 1);
	void Update(Transform* parent = nullptr);
	void Draw(const std::string layerTag, const BlendMode blendMode = BlendMode::Alpha);

public:
	template<typename T>
	void AddCSConstantBuffer()
	{
		std::unique_ptr<IConstantBuffer> iConstatnBuffer = std::make_unique<ConstantBuffer<T>>();
		iConstatnBuffer->Create();
		mCSMaterial->constantBuffers.push_back(std::move(iConstatnBuffer));
	}

	// ComputeShaderのみで使うデータを増やす関数
	template<typename T>
	void AddStructuredBuffer()
	{
		// StructuredBufferの作成
		mCSMaterial->structuredBuffers.push_back(std::move(std::make_unique<StructuredBuffer>()));
		mCSMaterial->structuredBuffers.back()->Create(sizeof(T));

		// UAV作成
		DescriptorHeapManager::GetDescriptorHeap("SRV")->
			CreateUAV(mCSMaterial->structuredBuffers.back()->GetBufferResource(), 1, sizeof(T));
	}

	template<typename T>
	void TransferCSConstantBuffer(const uint32_t bufferNum, const T& data)
	{
		uint32_t bNum = Min<uint32_t>(bufferNum, (uint32_t)mCSMaterial->constantBuffers.size());
		TransferDataToConstantBuffer(mCSMaterial->constantBuffers[bNum].get(), data);
	}

public: // セッター

	// パーティクルのテクスチャー
	void SetParticleTexture(Texture* texture);

	// Graphicsパイプラインを設定
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

	// Computeパイプラインを設定
	void SetComputePipeline(ComputePipeline* computePipeline);

	// パーティクルのデータ
	template<typename T>
	void SetParticleData(Model* model, const uint32_t meshParticleSize = 0)
	{
		// モデルセット
		mModel = model;

		// 頂点バッファでSRV作成
		DescriptorHeapManager::GetDescriptorHeap("SRV")->
			CreateSRV(model->mesh.vertexBuffer.GetBufferResource(),
				(uint32_t)model->mesh.vertices.size(), sizeof(model->mesh.vertices.front()));

		uint32_t triangleSize = (uint32_t)model->mesh.vertices.size() / 3;
		mMeshParticleSize = meshParticleSize;
		mMaxParticleSize = mMeshParticleSize * triangleSize;

		// SRVとUAVを作成
		uint32_t dataSize = sizeof(T) * mMaxParticleSize;
		mParticleData->Create(dataSize);

		DescriptorHeapManager::GetDescriptorHeap("SRV")->
			CreateSRV(mParticleData->GetBufferResource(), mMaxParticleSize, sizeof(T));

		// GENERIC_READ -> UNORDERED_ACCESS に変更
		RenderBase::GetInstance()->TransitionBufferState(
			mParticleData->GetBufferResource(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		mParticleData->GetBufferResource()->buffer->GetDesc();

		DescriptorHeapManager::GetDescriptorHeap("SRV")->
			CreateUAV(mParticleData->GetBufferResource(), mMaxParticleSize, sizeof(T));
	}

public: // ゲッター
	Vec3 GetWorldPos();
	Vec3 GetWorldScale();
	Transform GetTransform();
};
