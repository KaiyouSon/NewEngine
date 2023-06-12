#include "CircleGaugeSprite.h"
#include "Camera.h"

using namespace ConstantBufferData;

CircleGaugeSprite::CircleGaugeSprite() :
	pos(0), scale(1), rot(0), anchorPoint(0.5f),
	startRadian(0), endRadian(0),
	vertexBuffer_(std::make_unique<VertexBuffer<VertexPosUv>>()),
	constantBufferTransform_(std::make_unique<ConstantBuffer<CTransform2D>>()),
	constantBufferColor_(std::make_unique<ConstantBuffer<CColor>>()),
	constantBufferCircleGauge_(std::make_unique<ConstantBuffer<CCircleGauge>>()),
	graphicsPipeline_(GraphicsPipelineManager::GetGraphicsPipeline("CircleGaugeSprite"))
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
	constantBufferCircleGauge_->Init();
}

void CircleGaugeSprite::Update()
{
	transform_.pos = pos;
	transform_.scale = { scale.x,scale.y,1 };
	transform_.rot = { 0,0,rot };
	transform_.Update();

	endRadian = Clamp<float>(endRadian, 0, Radian(360));

	// �萔�o�b�t�@�ɓ]��
	constantBufferTransform_->constantBufferMap->mat =
		transform_.GetWorldMat() *
		Camera::current.GetOrthoGrphicProjectionMat();

	// �F�]��
	constantBufferColor_->constantBufferMap->color = color / 255;
	constantBufferColor_->constantBufferMap->color.a = color.a / 255;

	// �F�]��
	constantBufferCircleGauge_->constantBufferMap->startRadian = startRadian;
	constantBufferCircleGauge_->constantBufferMap->endRadian = endRadian + startRadian;

	// ���_�o�b�t�@�[�ɒ��_��]��
	TransferTexturePos();
}

void CircleGaugeSprite::Draw(const BlendMode blendMode)
{
	SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer_->GetvbViewAddress());

	// �}�e���A���ƃg�����X�t�H�[����CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform_->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferColor_->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, constantBufferCircleGauge_->constantBuffer->GetGPUVirtualAddress());

	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		renderBase->GetSpriteRootSignature()->GetRootDescriptorTableIndex(), texture->GetGpuHandle());

	renderBase->GetCommandList()->DrawInstanced((unsigned short)vertices_.size(), 1, 0, 0);
}

void CircleGaugeSprite::SetBlendMode(const BlendMode blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();//.get();

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

void CircleGaugeSprite::TransferTexturePos()
{
	// �V�����̃T�C�Y
	float width = texture->size.x;
	float height = texture->size.y;

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