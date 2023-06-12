#include "Sprite.h"
#include "MathUtil.h"
#include "RenderBase.h"
#include "TextureManager.h"
#include "Camera.h"
using namespace ConstantBufferData;

Sprite::Sprite() :
	texture(TextureManager::GetTexture("White")),
	pos(0), scale(1), size(0), rot(0), color(Color::white), anchorPoint(0.5f),
	vertexBuffer_(std::make_unique<VertexBuffer<VertexPosUv>>()),
	constantBufferTransform_(std::make_unique<ConstantBuffer<CTransform2D>>()),
	constantBufferColor_(std::make_unique<ConstantBuffer<CColor>>()),
	graphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Sprite"))
{
	vertices_.resize(4);
	vertices_[0].uv = { 0.0f,1.0f };
	vertices_[1].uv = { 0.0f,0.0f };
	vertices_[2].uv = { 1.0f,1.0f };
	vertices_[3].uv = { 1.0f,0.0f };
	vertexBuffer_->Create(vertices_);

	// �萔�o�b�t�@
	constantBufferTransform_->Init();
	constantBufferColor_->Init();
}

void Sprite::Update(Transform* parent)
{
	transform_.pos = pos;
	transform_.scale = { scale.x,scale.y,1 };
	transform_.rot = { 0,0,rot };
	transform_.Update();

	if (parent != nullptr)
	{
		parent_ = parent;

		Mat4 mat = transform_.GetWorldMat();
		mat *= parent_->GetWorldMat();
		transform_.SetWorldMat(mat);
	}

	// �萔�o�b�t�@�ɓ]��
	constantBufferTransform_->constantBufferMap->mat =
		transform_.GetWorldMat() *
		Camera::current.GetOrthoGrphicProjectionMat();

	// �F�]��
	constantBufferColor_->constantBufferMap->color = color / 255;
	constantBufferColor_->constantBufferMap->color.a = color.a / 255;

	// ���_�o�b�t�@�[�ɒ��_��]��
	TransferTexturePos(size);
}

void Sprite::Draw(const BlendMode blendMode)
{
	SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	//renderBase->GetCommandList()->SetPipelineState(renderBase->GetSpritePipeline()->GetAlphaPipeline());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer_->GetvbViewAddress());

	// �}�e���A���ƃg�����X�t�H�[����CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform_->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferColor_->constantBuffer->GetGPUVirtualAddress());

	size_t max = renderBase->GetSpriteRootSignature()->GetConstantBufferNum();

	for (int i = 0; i < 1; i++)
	{
		// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
			UINT(max + i), texture->GetGpuHandle());
	}

	renderBase->GetCommandList()->DrawInstanced((unsigned short)vertices_.size(), 1, 0, 0);
}

void Sprite::SetBlendMode(const BlendMode blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();//.get();

	switch (blendMode)
	{
	case BlendMode::Alpha: // ���u�����h
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline->GetAlphaPipeline());
		break;

	case BlendMode::Add:	// ���Z�u�����h
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline->GetAddPipeline());
		break;

	case BlendMode::Sub:	// ���Z�u�����h
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline->GetSubPipeline());
		break;

	case BlendMode::Inv:	// ���]
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline->GetInvPipeline());
		break;

	default:
		break;
	}
}

void Sprite::TransferTexturePos(const Vec2& size)
{
	// �V�����̃T�C�Y
	float width = size.x == 0 ? texture->size.x : size.x;
	float height = size.y == 0 ? texture->size.y : size.y;

	// ���݂̃T�C�Y
	float width2 = vertices_[0].pos.x - vertices_[2].pos.x;
	float height2 = vertices_[1].pos.x - vertices_[3].pos.x;

	if (width != fabsf(width2) || width != fabsf(height2))
	{
		vertices_[0].pos = { (0.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //����
		vertices_[1].pos = { (0.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //����
		vertices_[2].pos = { (1.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //�E��
		vertices_[3].pos = { (1.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //�E��

		vertexBuffer_->TransferToBuffer(vertices_);
	}
}

void Sprite::SetTextureRect(const Vec2 leftTopPos, const Vec2 rightDownPos)
{
	float left = leftTopPos.x / texture->buffer->GetDesc().Width;
	float right = rightDownPos.x / texture->buffer->GetDesc().Width;
	float up = leftTopPos.y / texture->buffer->GetDesc().Height;
	float down = rightDownPos.y / texture->buffer->GetDesc().Height;

	vertices_[0].uv = { left,down };
	vertices_[1].uv = { left,up };
	vertices_[2].uv = { right,down };
	vertices_[3].uv = { right,up };

	vertexBuffer_->TransferToBuffer(vertices_);
}