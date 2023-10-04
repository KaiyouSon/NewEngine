#include "Emitter.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

Emitter::Emitter() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	offset(0, 0), tiling(1, 1), pSize(0),
	mVertexBuffer(std::make_unique <VertexBuffer<VParticle>>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Emitter")),
	mTexture(TextureManager::GetTexture("White")),

	mStructuredBuffer(std::make_unique<StructuredBuffer<ParticleParameter::Test>>()),
	mRWStructuredBuffer(std::make_unique<RWStructuredBuffer<ParticleParameter::Test>>())
{
	// マテリアルの初期化
	MaterialInit();
	mTexture->isMaterial = true;

	mStructuredBuffer->Create();
	mRWStructuredBuffer->Create();
	DescriptorHeapManager::GetDescriptorHeap("SRV")->
		CreateSRV(mStructuredBuffer->GetBufferResource(), 1, sizeof(ParticleParameter::Test));

	CD3DX12_RESOURCE_BARRIER barrier =
		CD3DX12_RESOURCE_BARRIER::Transition(
			mStructuredBuffer->GetBufferResource()->buffer.Get(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
	RenderBase::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	DescriptorHeapManager::GetDescriptorHeap("SRV")->
		CreateUAV(mStructuredBuffer->GetBufferResource(), 1, sizeof(ParticleParameter::Test));

	mBillboard.SetBillboardType(BillboardType::AllAxisBillboard);
}

void Emitter::Update(Transform* parent)
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


	for (uint32_t i = 0; i < pSize; i++)
	{
		mVertices[i].pos = pParam[i].curPos;
		mVertices[i].scale = pParam[i].curScale;
		mVertices[i].rot = pParam[i].curRot;
		mVertices[i].shininess = pParam[i].curShininess;
		mVertices[i].color = pParam[i].curColor.To01();
	}
	mVertexBuffer->TransferToBuffer(mVertices);
}
void Emitter::Draw(const BlendMode blendMode)
{
	if (mTexture == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	PipelineManager::GetComputePipeline("Emitter")->ExecuteCommand();

	ID3D12DescriptorHeap* descriptorHeap[] =
	{
		DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap()
	};
	cmdList->SetDescriptorHeaps(1, descriptorHeap);

	uint32_t index = PipelineManager::GetComputePipeline("Emitter")->GetRootSignature()->GetSRVStartIndex();
	cmdList->SetComputeRootDescriptorTable(0, mStructuredBuffer->GetBufferResource()->srvHandle.gpu);

	uint32_t end = PipelineManager::GetComputePipeline("Emitter")->GetRootSignature()->GetUAVStartIndex();
	cmdList->SetComputeRootDescriptorTable(1, mStructuredBuffer->GetBufferResource()->uavHandle.gpu);

	// ディスパッチ
	cmdList->Dispatch(1, 1, 1);

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBVとIBVの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	ID3D12DescriptorHeap* descriptorHeap2[] =
	{
		DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap()
	};
	cmdList->SetDescriptorHeaps(1, descriptorHeap2);

	MaterialDrawCommands();

	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		startIndex, mTexture->GetBufferResource()->srvHandle.gpu);

	auto ViewDimension = mStructuredBuffer->GetBufferResource()->buffer->GetDesc().Dimension;
	auto Format = mStructuredBuffer->GetBufferResource()->buffer->GetDesc().Format;

	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		startIndex + 1, mStructuredBuffer->GetBufferResource()->srvHandle.gpu);

	//renderBase->GetCommandList()->DrawInstanced(pSize, 1, 0, 0);
	renderBase->GetCommandList()->DrawInstanced((uint32_t)mVertices.size(), 1, 0, 0);

}

// --- マテリアル関連 --------------------------------------------------- //
void Emitter::MaterialInit()
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
void Emitter::MaterialTransfer()
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
void Emitter::MaterialDrawCommands()
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
void Emitter::SetTexture(Texture* texture) { mTexture = texture; }

// グラフィックスパイプライン
void Emitter::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { mGraphicsPipeline = graphicsPipeline; }

// パーティクルの数
void Emitter::SetMaxParticle(const uint32_t max)
{
	pParam.resize(max);
	mVertices.resize(max);
	mVertexBuffer->Create(mVertices);
}

// --- ゲッター -------------------------------------------------------- //

// ワールド座標
Vec3 Emitter::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform.GetWorldMat(), true);
	return worldPos;
}

// ワールドスケール
Vec3 Emitter::GetWorldScale()
{
	Vec3 worldScale = mTransform.GetWorldMat().GetScale();
	return worldScale;
}

// トランスフォーム
Transform Emitter::GetTransform() { return mTransform; }
