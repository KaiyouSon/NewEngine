#include "ParticleObject.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
#include "NewEngine.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

ParticleObject::ParticleObject() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	offset(0, 0), tiling(1, 1),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ParticleObject")),
	mComputePipeline(PipelineManager::GetComputePipeline("ParticleObject")),
	mParticleTexture(TextureManager::GetTexture("White")),
	mParticleData(std::make_unique<StructuredBuffer>()),
	mCSMaterial(std::make_unique<Material>())
{
	// マテリアルの初期化
	MaterialInit();
	CSMaterialInit();

	mBillboard.SetBillboardType(BillboardType::AllAxisBillboard);
}
ParticleObject::~ParticleObject()
{
	if (NewEngine::GetisClose() == true)
	{
		return;
	}

	auto* descriptorHeap = DescriptorHeapManager::GetDescriptorHeap("SRV");

	// ビュー解放
	descriptorHeap->DestroyView(mModel->mesh.vertexBuffer.GetBufferResource());
	descriptorHeap->DestroyView(mParticleData->GetBufferResource());
	for (uint32_t i = 0; i < mCSMaterial->structuredBuffers.size(); i++)
	{
		descriptorHeap->DestroyView(mCSMaterial->structuredBuffers[i]->GetBufferResource());
	}
}

void ParticleObject::ExecuteCS(const uint32_t threadX, const uint32_t threadY, const uint32_t threadZ)
{
	if (mModel == nullptr)
	{
		return;
	}

	RenderBase* renderBase = RenderBase::GetInstance();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	CSMaterialTransfer();

	mComputePipeline->DrawCommand();

	CSMaterialDrawCommands();

	// ディスパッチ
	cmdList->Dispatch(threadX, threadY, threadZ);
}
void ParticleObject::Update(Transform* parent)
{
	mTransform.pos = pos;
	mTransform.scale = scale;
	mTransform.rot = rot;
	mTransform.Update();

	if (parent != nullptr)
	{
		mParent = parent;

		Mat4 mat = mTransform.GetWorldMat();
		mat *= mParent->GetWorldMat();
		mTransform.SetWorldMat(mat);
	}

	// マテリアルの転送
	MaterialTransfer();

}
void ParticleObject::Draw(const BlendMode blendMode)
{
	if (mParticleTexture == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

	MaterialDrawCommands();

	// SRVの設定
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	cmdList->SetGraphicsRootDescriptorTable(startIndex, mParticleTexture->GetBufferResource()->srvHandle.gpu);

	if (mParticleData->GetBufferResource()->bufferState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
	{
		// GENERIC_READ -> UNORDERED_ACCESS に変更
		renderBase->TransitionBufferState(
			mParticleData->GetBufferResource(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_GENERIC_READ);
	}

	// CSの結果
	cmdList->SetGraphicsRootDescriptorTable(
		startIndex + 1, mParticleData->GetBufferResource()->srvHandle.gpu);

	cmdList->DrawInstanced(mMaxParticleSize, 1, 0, 0);
}

// --- マテリアル関連 --------------------------------------------------- //
void ParticleObject::MaterialInit()
{
	// インターフェース
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// トランスフォーム
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformP>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// UVパラメーター
	iConstantBuffer = std::make_unique<ConstantBuffer<CUVParameter>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial.Init();
}
void ParticleObject::MaterialTransfer()
{
	mBillboard.CalculateBillboardMat();

	// トランスフォーム
	CTransformP transformPData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
		mBillboard.GetMat(),
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transformPData);

	// 色
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);

	// UVパラメーター
	CUVParameter uvData = { offset,tiling };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[2].get(), uvData);
}
void ParticleObject::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBV縺ｮ險ｭ螳壹さ繝槭Φ繝・
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

void ParticleObject::CSMaterialInit()
{
	// インターフェース
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// テクスチャーの面積
	iConstantBuffer = std::make_unique<ConstantBuffer<CParticleObject>>();
	mCSMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	mCSMaterial->Init();
}
void ParticleObject::CSMaterialTransfer()
{
	// メッシュテクスチャ情報
	CParticleObject cParticleObject =
	{
		static_cast<uint32_t>(mModel->mesh.vertices.size()),
		static_cast<uint32_t>(mMeshParticleSize),
		static_cast<uint32_t>(mMaxParticleSize),
	};
	TransferDataToConstantBuffer(mCSMaterial->constantBuffers[0].get(), cParticleObject);
}
void ParticleObject::CSMaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();
	ID3D12GraphicsCommandList* cmdList = RenderBase::GetInstance()->GetCommandList();

	// CBV
	uint32_t cbvStartIndex = mComputePipeline->GetRootSignature()->GetCBVStartIndex();
	for (uint32_t i = 0; i < mCSMaterial->constantBuffers.size(); i++)
	{
		cmdList->SetComputeRootConstantBufferView(
			cbvStartIndex + i,
			mCSMaterial->constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}

	// SRV
	uint32_t srvStartIndex = mComputePipeline->GetRootSignature()->GetSRVStartIndex();
	cmdList->SetComputeRootDescriptorTable(srvStartIndex, mModel->mesh.vertexBuffer.GetBufferResource()->srvHandle.gpu);

	if (mParticleData->GetBufferResource()->bufferState == D3D12_RESOURCE_STATE_GENERIC_READ)
	{
		// GENERIC_READ -> UNORDERED_ACCESS に変更
		renderBase->TransitionBufferState(
			mParticleData->GetBufferResource(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	}

	// UAV
	uint32_t uavStartIndex = mComputePipeline->GetRootSignature()->GetUAVStartIndex();
	cmdList->SetComputeRootDescriptorTable(uavStartIndex, mParticleData->GetBufferResource()->uavHandle.gpu);
	// その他のUAVデータ
	for (uint32_t i = 0; i < mCSMaterial->structuredBuffers.size(); i++)
	{
		cmdList->SetComputeRootDescriptorTable(
			uavStartIndex + i + 1,
			mCSMaterial->structuredBuffers[i]->GetBufferResource()->uavHandle.gpu);
	}
}

// --- セッター -------------------------------------------------------- //

// パーティクルのテクスチャー
void ParticleObject::SetParticleTexture(Texture* particleTexture) { mParticleTexture = particleTexture; }

// Graphicsパイプラインを設定
void ParticleObject::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { mGraphicsPipeline = graphicsPipeline; }

// Computeパイプラインを設定
void ParticleObject::SetComputePipeline(ComputePipeline* computePipeline) { mComputePipeline = computePipeline; }

// --- ゲッター -------------------------------------------------------- //

Vec3 ParticleObject::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform.GetWorldMat(), true);
	return worldPos;
}

Vec3 ParticleObject::GetWorldScale()
{
	Vec3 worldScale = mTransform.GetWorldMat().GetScale();
	return worldScale;
}

Transform ParticleObject::GetTransform()
{
	return mTransform;
}
