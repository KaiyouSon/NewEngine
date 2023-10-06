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
	// �}�e���A���̏�����
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

	// �}�e���A���̓]��
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
		// GENERIC_READ -> UNORDERED_ACCESS �ɂ���SRV��ݒ肷��
		renderBase->TransitionBufferState(
			mStructuredBuffer->GetBufferResource(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	}

	// UAV
	uint32_t index = mComputePipeline->GetRootSignature()->GetUAVStartIndex();
	cmdList->SetComputeRootDescriptorTable(index, mStructuredBuffer->GetBufferResource()->uavHandle.gpu);

	// �f�B�X�p�b�`
	cmdList->Dispatch(1, 1, 1);

	// GraphicsPipeline�`��R�}���h
	mGraphicsPipeline->DrawCommand(blendMode);

	auto descriptorHeap2 = DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap();
	cmdList->SetDescriptorHeaps(1, &descriptorHeap2);

	MaterialDrawCommands();

	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	cmdList->SetGraphicsRootDescriptorTable(
		startIndex, mTexture->GetBufferResource()->srvHandle.gpu);

	if (mStructuredBuffer->GetBufferResource()->bufferState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
	{
		// GENERIC_READ -> UNORDERED_ACCESS �ɂ���SRV��ݒ肷��
		renderBase->TransitionBufferState(
			mStructuredBuffer->GetBufferResource(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_GENERIC_READ);
	}

	cmdList->SetGraphicsRootDescriptorTable(
		startIndex + 1, mStructuredBuffer->GetBufferResource()->srvHandle.gpu);

	cmdList->DrawInstanced((uint32_t)mVertices.size(), 1, 0, 0);
}

// --- �}�e���A���֘A --------------------------------------------------- //
void GPUEmitter::MaterialInit()
{
	// �C���X�^���X����
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D�s��
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformP>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// �F
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// UV���
	iConstantBuffer = std::make_unique<ConstantBuffer<CUVParameter>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// ������
	mMaterial.Init();
}
void GPUEmitter::MaterialTransfer()
{
	mBillboard.CalculateBillboardMat();

	// �}�g���b�N�X
	CTransformP transformPData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
		mBillboard.GetMat(),
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transformPData);

	// �F�f�[�^
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);

	// UV�f�[�^
	CUVParameter uvData = { offset,tiling };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[2].get(), uvData);
}
void GPUEmitter::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBV�̐ݒ�R�}���h
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- �Z�b�^�[ -------------------------------------------------------- //ko

// �e�N�X�`���[
void GPUEmitter::SetTexture(Texture* texture) { mTexture = texture; }

// �O���t�B�b�N�X�p�C�v���C��
void GPUEmitter::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { mGraphicsPipeline = graphicsPipeline; }

// �p�[�e�B�N���̐�
void GPUEmitter::SetMaxParticle(const uint32_t max)
{
	pParam.resize(max);
	mVertices.resize(max);

	// SRV��UAV���쐬
	ParticleParameter::PParam0 particleData[32];

	uint32_t dataSize = sizeof(ParticleParameter::PParam0) * max;
	mStructuredBuffer->Create(dataSize);

	DescriptorHeapManager::GetDescriptorHeap("SRV")->
		CreateSRV(mStructuredBuffer->GetBufferResource(), max, sizeof(ParticleParameter::PParam0));

	// GENERIC_READ -> UNORDERED_ACCESS �ɂ���UAV���쐬
	RenderBase::GetInstance()->TransitionBufferState(
		mStructuredBuffer->GetBufferResource(),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	mStructuredBuffer->GetBufferResource()->buffer->GetDesc();

	DescriptorHeapManager::GetDescriptorHeap("SRV")->
		CreateUAV(mStructuredBuffer->GetBufferResource(), max, sizeof(ParticleParameter::PParam0));
}

// --- �Q�b�^�[ -------------------------------------------------------- //

// ���[���h���W
Vec3 GPUEmitter::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform.GetWorldMat(), true);
	return worldPos;
}

// ���[���h�X�P�[��
Vec3 GPUEmitter::GetWorldScale()
{
	Vec3 worldScale = mTransform.GetWorldMat().GetScale();
	return worldScale;
}

// �g�����X�t�H�[��
Transform GPUEmitter::GetTransform() { return mTransform; }
