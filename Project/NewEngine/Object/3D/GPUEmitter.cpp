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
	// 繝槭ユ繝ｪ繧｢繝ｫ縺ｮ蛻晄悄蛹・
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

	// 繝槭ユ繝ｪ繧｢繝ｫ縺ｮ霆｢騾・
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
		// GENERIC_READ -> UNORDERED_ACCESS 縺ｫ縺励※SRV繧定ｨｭ螳壹☆繧・
		renderBase->TransitionBufferState(
			mStructuredBuffer->GetBufferResource(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	}

	// UAV
	uint32_t index = mComputePipeline->GetRootSignature()->GetUAVStartIndex();
	cmdList->SetComputeRootDescriptorTable(index, mStructuredBuffer->GetBufferResource()->uavHandle.gpu);

	// 繝・ぅ繧ｹ繝代ャ繝・
	cmdList->Dispatch(1, 1, 1);

	// GraphicsPipeline謠冗判繧ｳ繝槭Φ繝・
	mGraphicsPipeline->DrawCommand(blendMode);

	auto descriptorHeap2 = DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap();
	cmdList->SetDescriptorHeaps(1, &descriptorHeap2);

	MaterialDrawCommands();

	// SRV繝偵・繝励・蜈磯ｭ縺ｫ縺ゅｋSRV繧偵Ν繝ｼ繝医ヱ繝ｩ繝｡繝ｼ繧ｿ2逡ｪ縺ｫ險ｭ螳・
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	cmdList->SetGraphicsRootDescriptorTable(
		startIndex, mTexture->GetBufferResource()->srvHandle.gpu);

	if (mStructuredBuffer->GetBufferResource()->bufferState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
	{
		// GENERIC_READ -> UNORDERED_ACCESS 縺ｫ縺励※SRV繧定ｨｭ螳壹☆繧・
		renderBase->TransitionBufferState(
			mStructuredBuffer->GetBufferResource(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_GENERIC_READ);
	}

	cmdList->SetGraphicsRootDescriptorTable(
		startIndex + 1, mStructuredBuffer->GetBufferResource()->srvHandle.gpu);

	cmdList->DrawInstanced((uint32_t)mVertices.size(), 1, 0, 0);
}

// --- 繝槭ユ繝ｪ繧｢繝ｫ髢｢騾｣ --------------------------------------------------- //
void GPUEmitter::MaterialInit()
{
	// 繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ逕滓・
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D陦悟・
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformP>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 濶ｲ
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// UV諠・ｱ
	iConstantBuffer = std::make_unique<ConstantBuffer<CUVParameter>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 蛻晄悄蛹・
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

// --- 繧ｻ繝・ち繝ｼ -------------------------------------------------------- //ko

// 繝・け繧ｹ繝√Ε繝ｼ
void GPUEmitter::SetTexture(Texture* texture) { mTexture = texture; }

// 繧ｰ繝ｩ繝輔ぅ繝・け繧ｹ繝代う繝励Λ繧､繝ｳ
void GPUEmitter::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { mGraphicsPipeline = graphicsPipeline; }

// 繝代・繝・ぅ繧ｯ繝ｫ縺ｮ謨ｰ
void GPUEmitter::SetMaxParticle(const uint32_t max)
{
	pParam.resize(max);
	mVertices.resize(max);

	// SRV縺ｨUAV繧剃ｽ懈・
	uint32_t dataSize = sizeof(ParticleParameter::PParam0) * max;
	mStructuredBuffer->Create(dataSize);

	DescriptorHeapManager::GetDescriptorHeap("SRV")->
		CreateSRV(mStructuredBuffer->GetBufferResource(), max, sizeof(ParticleParameter::PParam0));

	// GENERIC_READ -> UNORDERED_ACCESS 縺ｫ縺励※UAV繧剃ｽ懈・
	RenderBase::GetInstance()->TransitionBufferState(
		mStructuredBuffer->GetBufferResource(),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	mStructuredBuffer->GetBufferResource()->buffer->GetDesc();

	DescriptorHeapManager::GetDescriptorHeap("SRV")->
		CreateUAV(mStructuredBuffer->GetBufferResource(), max, sizeof(ParticleParameter::PParam0));
}

// --- 繧ｲ繝・ち繝ｼ -------------------------------------------------------- //

// 繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓・
Vec3 GPUEmitter::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform.GetWorldMat(), true);
	return worldPos;
}

// 繝ｯ繝ｼ繝ｫ繝峨せ繧ｱ繝ｼ繝ｫ
Vec3 GPUEmitter::GetWorldScale()
{
	Vec3 worldScale = mTransform.GetWorldMat().GetScale();
	return worldScale;
}

// 繝医Λ繝ｳ繧ｹ繝輔か繝ｼ繝
Transform GPUEmitter::GetTransform()
{
	return mTransform;
}
