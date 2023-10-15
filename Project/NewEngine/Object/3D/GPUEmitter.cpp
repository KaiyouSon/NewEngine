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
	offset(0, 0), tiling(1, 1),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GPUEmitter")),
	mComputePipeline(PipelineManager::GetComputePipeline("GPUEmitter")),
	mTexture(TextureManager::GetTexture("White")),
	mParticleData(std::make_unique<StructuredBuffer>())
{
	// マテリアルの初期化
	MaterialInit();
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

	// 繝槭ユ繝ｪ繧｢繝ｫ縺ｮ霆｢騾・
	MaterialTransfer();
}
void GPUEmitter::Draw(const BlendMode blendMode)
{
	if (mTexture == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	mComputePipeline->DrawCommand();

	if (mParticleData->GetBufferResource()->bufferState == D3D12_RESOURCE_STATE_GENERIC_READ)
	{
		// GENERIC_READ -> UNORDERED_ACCESS に変更
		renderBase->TransitionBufferState(
			mParticleData->GetBufferResource(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	}

	// UAV
	uint32_t index = mComputePipeline->GetRootSignature()->GetUAVStartIndex();
	cmdList->SetComputeRootDescriptorTable(index, mParticleData->GetBufferResource()->uavHandle.gpu);

	// その他のデータ
	for (uint32_t i = 0; i < mStructuredBuffers.size(); i++)
	{
		cmdList->SetComputeRootDescriptorTable(index + i + 1, mStructuredBuffers[i]->GetBufferResource()->uavHandle.gpu);
	}

	// ディスパッチ
	cmdList->Dispatch(1, 1, 1);

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

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

// --- マテリアル関連 --------------------------------------------------- //
void GPUEmitter::MaterialInit()
{
	// インターフェース
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// トランスフォーム
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformP>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// UVパラメータ
	iConstantBuffer = std::make_unique<ConstantBuffer<CUVParameter>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial.Init();
}
void GPUEmitter::MaterialTransfer()
{
	mBillboard.CalculateBillboardMat();

	// 繝槭ヨ繝ｪ繝・け繧ｹ
	CTransformP transformPData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
		mBillboard.GetMat(),
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transformPData);

	// 濶ｲ繝・・繧ｿ
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);

	// UV繝・・繧ｿ
	CUVParameter uvData = { offset,tiling };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[2].get(), uvData);
}
void GPUEmitter::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBV縺ｮ險ｭ螳壹さ繝槭Φ繝・
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- セッター --------------------------------------------------------
void GPUEmitter::SetTexture(Texture* texture) { mTexture = texture; }
void GPUEmitter::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { mGraphicsPipeline = graphicsPipeline; }
void GPUEmitter::SetComputePipeline(ComputePipeline* computePipeline) { mComputePipeline = computePipeline; }

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
