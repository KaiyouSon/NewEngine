#include "GPUEmitter.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
#include "NewEngine.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

GPUEmitter::GPUEmitter() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	offset(0, 0), tiling(1, 1), mMaxParticle(0),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GPUEmitter")),
	mComputePipeline(PipelineManager::GetComputePipeline("GPUEmitter")),
	mTexture(TextureManager::GetTexture("White")),
	mParticleData(std::make_unique<StructuredBuffer>()),
	mMaterial(std::make_unique<Material>()),
	mCSMaterial(std::make_unique<Material>())
{
	// マテリアルの初期化
	MaterialInit();
	CSMaterialInit();

	mBillboard.SetBillboardType(BillboardType::AllAxisBillboard);
}
GPUEmitter::~GPUEmitter()
{
	if (NewEngine::GetisClose() == true)
	{
		return;
	}

	auto* descriptorHeap = DescriptorHeapManager::GetDescriptorHeap("SRV");

	// ビュー解放
	descriptorHeap->DestroyView(mParticleData->GetBufferResource());
	for (uint32_t i = 0; i < mStructuredBuffers.size(); i++)
	{
		descriptorHeap->DestroyView(mStructuredBuffers[i]->GetBufferResource());
	}
}

void GPUEmitter::ExecuteCS(const uint32_t threadX, const uint32_t threadY, const uint32_t threadZ)
{
	RenderBase* renderBase = RenderBase::GetInstance();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	CSMaterialTransfer();

	mComputePipeline->DrawCommand();

	CSMaterialDrawCommands();

	// ディスパッチ
	cmdList->Dispatch(threadX, threadY, threadZ);
}
void GPUEmitter::Update(Transform* parent)
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
void GPUEmitter::Draw(const std::string layerTag, const BlendMode blendMode)
{
	mBlendMode = blendMode;

	//Renderer::GetInstance()->Register(layerTag,
	//	[this]()
	//	{
	//		DrawCommands();
	//	});

	DrawCommands();
}

// --- マテリアル関連 --------------------------------------------------- //
void GPUEmitter::MaterialInit()
{
	// インターフェース
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// トランスフォーム
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformP>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// UVパラメータ
	iConstantBuffer = std::make_unique<ConstantBuffer<CUVParameter>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial->Init();
}
void GPUEmitter::MaterialTransfer()
{
	mBillboard.CalculateBillboardMat();

	// トランスフォーム
	CTransformP transformPData =
	{
		Camera::current.GetViewMat() * Camera::current.GetPerspectiveMat(),
		mTransform.GetWorldMat(),
		mBillboard.GetMat(),
	};
	TransferDataToConstantBuffer(mMaterial->constantBuffers[0].get(), transformPData);

	// 色
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[1].get(), colorData);

	// UVパラメーター
	CUVParameter uvData = { offset,tiling };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[2].get(), uvData);
}
void GPUEmitter::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial->constantBuffers.size(); i++)
	{
		// CBVセット
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial->constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}
void GPUEmitter::CSMaterialInit()
{
	// インターフェース
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// パーティクルのマックスサイズ
	iConstantBuffer = std::make_unique<ConstantBuffer<CMaxParticle>>();
	mCSMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	mCSMaterial->Init();
}
void GPUEmitter::CSMaterialTransfer()
{
	// メッシュテクスチャ情報
	CMaxParticle maxParticleSize = { mMaxParticle };
	TransferDataToConstantBuffer(mCSMaterial->constantBuffers[0].get(), maxParticleSize);
}
void GPUEmitter::CSMaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();
	ID3D12GraphicsCommandList* cmdList = RenderBase::GetInstance()->GetCommandList();

	// ディスクリプターヒープ設定
	auto srvDescHeap = DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap();
	cmdList->SetDescriptorHeaps(1, &srvDescHeap);

	// CBV
	uint32_t cbvStartIndex = mComputePipeline->GetRootSignature()->GetCBVStartIndex();
	for (uint32_t i = 0; i < mCSMaterial->constantBuffers.size(); i++)
	{
		cmdList->SetComputeRootConstantBufferView(
			cbvStartIndex + i, mCSMaterial->constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}

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
	for (uint32_t i = 0; i < mStructuredBuffers.size(); i++)
	{
		cmdList->SetComputeRootDescriptorTable(
			uavStartIndex + i + 1, mStructuredBuffers[i]->GetBufferResource()->uavHandle.gpu);
	}
}

void GPUEmitter::DrawCommands()
{
	if (mTexture == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(mBlendMode);

	// マテリアルの描画コマンド
	MaterialDrawCommands();

	// SRV設定
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	cmdList->SetGraphicsRootDescriptorTable(
		startIndex, mTexture->GetBufferResource()->srvHandle.gpu);

	if (mParticleData->GetBufferResource()->bufferState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
	{
		// GENERIC_READ -> UNORDERED_ACCESS に変更
		renderBase->TransitionBufferState(
			mParticleData->GetBufferResource(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_GENERIC_READ);
	}

	cmdList->SetGraphicsRootDescriptorTable(
		startIndex + 1, mParticleData->GetBufferResource()->srvHandle.gpu);

	cmdList->DrawInstanced((uint32_t)mVertices.size(), 1, 0, 0);
}

// --- セッター --------------------------------------------------------
void GPUEmitter::SetTexture(Texture* texture)
{
	mTexture = texture;
}
void GPUEmitter::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGraphicsPipeline = graphicsPipeline;
}
void GPUEmitter::SetComputePipeline(ComputePipeline* computePipeline)
{
	mComputePipeline = computePipeline;
}

// --- ゲッターｼ -------------------------------------------------------- //
Vec3 GPUEmitter::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform.GetWorldMat(), true);
	return worldPos;
}
Vec3 GPUEmitter::GetWorldScale()
{
	Vec3 worldScale = mTransform.GetWorldMat().GetScale();
	return worldScale;
}
Transform GPUEmitter::GetTransform()
{
	return mTransform;
}
