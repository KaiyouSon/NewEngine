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
	mGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Emitter")),
	mTexture(TextureManager::GetTexture("White")),

	mInputData(std::make_unique<StructuredBuffer<ParticleParameter::Test>>()),
	mOutputData(std::make_unique<RWStructuredBuffer<VertexBufferData::VParticle>>())
{
	// �}�e���A���̏�����
	MaterialInit();
	mTexture->isMaterial = true;

	mInputData->Create();
	mOutputData->Create();

	DescriptorHeapManager::GetDescriptorHeap("SRV_UAV")->
		CreateSRV(mInputData->GetBufferResource(), 1, sizeof(ParticleParameter::Test));
	DescriptorHeapManager::GetDescriptorHeap("SRV_UAV")->
		CreateUAV(mOutputData->GetBufferResource(), 1, sizeof(ParticleParameter::Test));

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

	// �}�e���A���̓]��
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

	//renderBase->GetCommandList()->
	//	SetComputeRootSignature(mGraphicsPipeline->GetRootSignature()->GetRootSignature());

	//renderBase->GetCommandList()->SetPipelineState(mGraphicsPipeline->GetPSO(blendMode));

	//renderBase->GetCommandList()->Dispatch(1, 1, 1);

	//uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetDescriptorTableStartIndex();
	//renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, mTexture->GetGpuHandle());



	// GraphicsPipeline�`��R�}���h
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	MaterialDrawCommands();

	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetDescriptorTableStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, mTexture->GetGpuHandle());

	renderBase->GetCommandList()->DrawInstanced(pSize, 1, 0, 0);

}

// --- �}�e���A���֘A --------------------------------------------------- //
void Emitter::MaterialInit()
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
void Emitter::MaterialTransfer()
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
void Emitter::MaterialDrawCommands()
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
void Emitter::SetTexture(Texture* texture) { mTexture = texture; }

// �O���t�B�b�N�X�p�C�v���C��
void Emitter::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { mGraphicsPipeline = graphicsPipeline; }

// �p�[�e�B�N���̐�
void Emitter::SetMaxParticle(const uint32_t max)
{
	pParam.resize(max);
	mVertices.resize(max);
	mVertexBuffer->Create(mVertices);
}

// --- �Q�b�^�[ -------------------------------------------------------- //

// ���[���h���W
Vec3 Emitter::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform.GetWorldMat(), true);
	return worldPos;
}

// ���[���h�X�P�[��
Vec3 Emitter::GetWorldScale()
{
	Vec3 worldScale = mTransform.GetWorldMat().GetScale();
	return worldScale;
}

// �g�����X�t�H�[��
Transform Emitter::GetTransform() { return mTransform; }
