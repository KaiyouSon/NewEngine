#include "GPUEmitter.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

GPUEmitter::GPUEmitter() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	offset(0, 0), tiling(1, 1), pSize(0),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GPUEmitter")),
	mComputePipeline(PipelineManager::GetComputePipeline("GPUEmitter")),
	mTexture(TextureManager::GetTexture("White")),
	mStructuredBuffer(std::make_unique<StructuredBuffer>())
{
	// マテリアルの初期化
	MaterialInit();
	mTexture->isMaterial = true;
	mBillboard.SetBillboardType(BillboardType::AllAxisBillboard);
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
void GPUEmitter::Draw(const BlendMode blendMode)
{
	if (mTexture == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	mComputePipeline->DrawCommand();

	auto descriptorHeaps1 = DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap();
	cmdList->SetDescriptorHeaps(1, &descriptorHeaps1);

	if (mStructuredBuffer->GetBufferResource()->bufferState == D3D12_RESOURCE_STATE_GENERIC_READ)
	{
		// GENERIC_READ -> UNORDERED_ACCESS にしてSRVを設定する
		renderBase->TransitionBufferState(
			mStructuredBuffer->GetBufferResource(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	}

	// UAV
	uint32_t index = mComputePipeline->GetRootSignature()->GetUAVStartIndex();
	cmdList->SetComputeRootDescriptorTable(index, mStructuredBuffer->GetBufferResource()->uavHandle.gpu);

	// ディスパッチ
	cmdList->Dispatch(1, 1, 1);

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

	auto descriptorHeap2 = DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap();
	cmdList->SetDescriptorHeaps(1, &descriptorHeap2);

	MaterialDrawCommands();

	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	cmdList->SetGraphicsRootDescriptorTable(
		startIndex, mTexture->GetBufferResource()->srvHandle.gpu);

	if (mStructuredBuffer->GetBufferResource()->bufferState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
	{
		// GENERIC_READ -> UNORDERED_ACCESS にしてSRVを設定する
		renderBase->TransitionBufferState(
			mStructuredBuffer->GetBufferResource(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_GENERIC_READ);
	}

	cmdList->SetGraphicsRootDescriptorTable(
		startIndex + 1, mStructuredBuffer->GetBufferResource()->srvHandle.gpu);

	cmdList->DrawInstanced((uint32_t)mVertices.size(), 1, 0, 0);
}

// --- マテリアル関連 --------------------------------------------------- //
void GPUEmitter::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformP>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// UV情報
	iConstantBuffer = std::make_unique<ConstantBuffer<CUVParameter>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial.Init();
}
void GPUEmitter::MaterialTransfer()
{
	mBillboard.CalculateBillboardMat();

	// マトリックス
	CTransformP transformPData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
		mBillboard.GetMat(),
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transformPData);

	// 色データ
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);

	// UVデータ
	CUVParameter uvData = { offset,tiling };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[2].get(), uvData);
}
void GPUEmitter::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBVの設定コマンド
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- セッター -------------------------------------------------------- //ko

// テクスチャー
void GPUEmitter::SetTexture(Texture* texture) { mTexture = texture; }

// グラフィックスパイプライン
void GPUEmitter::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { mGraphicsPipeline = graphicsPipeline; }

// パーティクルの数
void GPUEmitter::SetMaxParticle(const uint32_t max)
{
	pParam.resize(max);
	mVertices.resize(max);

	// SRVとUAVを作成
	ParticleParameter::PParam0 particleData[32];

	uint32_t dataSize = sizeof(ParticleParameter::PParam0) * max;
	mStructuredBuffer->Create(dataSize);

	DescriptorHeapManager::GetDescriptorHeap("SRV")->
		CreateSRV(mStructuredBuffer->GetBufferResource(), max, sizeof(ParticleParameter::PParam0));

	// GENERIC_READ -> UNORDERED_ACCESS にしてUAVを作成
	RenderBase::GetInstance()->TransitionBufferState(
		mStructuredBuffer->GetBufferResource(),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	mStructuredBuffer->GetBufferResource()->buffer->GetDesc();

	DescriptorHeapManager::GetDescriptorHeap("SRV")->
		CreateUAV(mStructuredBuffer->GetBufferResource(), max, sizeof(ParticleParameter::PParam0));
}

// --- ゲッター -------------------------------------------------------- //

// ワールド座標
Vec3 GPUEmitter::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform.GetWorldMat(), true);
	return worldPos;
}

// ワールドスケール
Vec3 GPUEmitter::GetWorldScale()
{
	Vec3 worldScale = mTransform.GetWorldMat().GetScale();
	return worldScale;
}

// トランスフォーム
Transform GPUEmitter::GetTransform() { return mTransform; }
