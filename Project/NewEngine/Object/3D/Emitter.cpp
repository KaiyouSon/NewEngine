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
	vertexBuffer_(std::make_unique <VertexBuffer<VParticle>>()),
	graphicsPipeline_(GraphicsPipelineManager::GetGraphicsPipeline("Emitter")),
	texture_(TextureManager::GetTexture("Particle"))
{
	// �}�e���A���̏�����
	MaterialInit();
	texture_->isMaterial = true;

	//transform_.SetBillboardType(BillboardType::AllAxisBillboard);
	billboard_.SetBillboardType(BillboardType::AllAxisBillboard);
}

void Emitter::Update(Transform* parent)
{
	transform_.pos = pos;
	transform_.scale = scale;
	transform_.rot = rot;
	transform_.Update();

	if (parent != nullptr)
	{
		parent_ = parent;

		Mat4 mat = transform_.GetWorldMat();
		mat *= parent_->GetWorldMat();
		transform_.SetWorldMat(mat);
	}

	// �}�e���A���̓]��
	MaterialTransfer();

	for (uint32_t i = 0; i < pSize; i++)
	{
		vertices_[i].pos = pParam[i].curPos;
		vertices_[i].scale = pParam[i].curScale;
		vertices_[i].rot = pParam[i].curRot;
		vertices_[i].color = pParam[i].curColor.GetColorTo01();
	}
	vertexBuffer_->TransferToBuffer(vertices_);
}
void Emitter::Draw(const BlendMode blendMode)
{
	if (texture_ == nullptr) return;

	SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer_->GetvbViewAddress());

	MaterialDrawCommands();

	size_t index = renderBase->GetObject3DRootSignature()->GetConstantBufferNum();
	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable((UINT)index, texture_->GetGpuHandle());

	renderBase->GetCommandList()->DrawInstanced(pSize, 1, 0, 0);

}

// --- �}�e���A���֘A --------------------------------------------------- //
void Emitter::MaterialInit()
{
	// �C���X�^���X����
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D�s��
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformP>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	// �F
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	// UV���
	iConstantBuffer = std::make_unique<ConstantBuffer<CUVParameter>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	// ������
	material_.Init();
}
void Emitter::MaterialTransfer()
{
	billboard_.CalculateBillboardMat();

	// �}�g���b�N�X
	CTransformP transformPData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		transform_.GetWorldMat(),
		billboard_.GetMat(),
	};
	TransferDataToConstantBuffer(material_.constantBuffers[0].get(), transformPData);

	// �F�f�[�^
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(material_.constantBuffers[1].get(), colorData);

	// UV�f�[�^
	CUVParameter uvData = { offset,tiling };
	TransferDataToConstantBuffer(material_.constantBuffers[2].get(), uvData);
}
void Emitter::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, material_.constantBuffers[0]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, material_.constantBuffers[1]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, material_.constantBuffers[2]->constantBuffer->GetGPUVirtualAddress());
}

// --- �Z�b�^�[ -------------------------------------------------------- //

//  �u�����h
void Emitter::SetBlendMode(const BlendMode blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	switch (blendMode)
	{
	case BlendMode::Alpha: // ���u�����h
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetAlphaPipeline());
		break;

	case BlendMode::Add:	// ���Z�u�����h
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetAddPipeline());
		break;

	case BlendMode::Sub:	// ���Z�u�����h
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetSubPipeline());
		break;

	case BlendMode::Inv:	// ���]
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetInvPipeline());
		break;

	default:
		break;
	}
}

// �e�N�X�`���[
void Emitter::SetTexture(Texture* texture) { texture_ = texture; }

// �O���t�B�b�N�X�p�C�v���C��
void Emitter::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { graphicsPipeline_ = graphicsPipeline; }

// �p�[�e�B�N���̐�
void Emitter::SetMaxParticle(const uint32_t max)
{
	pParam.resize(max);
	vertices_.resize(max);
	vertexBuffer_->Create(vertices_);
}

// --- �Q�b�^�[ -------------------------------------------------------- //

// ���[���h���W
Vec3 Emitter::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, transform_.GetWorldMat(), true);
	return worldPos;
}

// ���[���h�X�P�[��
Vec3 Emitter::GetWorldScale()
{
	Vec3 worldScale = transform_.GetWorldMat().GetScale();
	return worldScale;
}

// �g�����X�t�H�[��
Transform Emitter::GetTransform() { return transform_; }
