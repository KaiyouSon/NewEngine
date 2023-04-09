#include "CircleGaugeSprite.h"
#include "Camera.h"

CircleGaugeSprite::CircleGaugeSprite() :
	pos(0), scale(1), rot(0), anchorPoint(0.5f),
	startRadian(0), endRadian(0),
	vertexBuffer(new VertexBuffer<VertexPosUv>),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform2D>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>),
	constantBufferCircleGauge(new ConstantBuffer<ConstantBufferDataCircleGauge>),
	graphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("CircleGaugeSprite"))
{
	vertices.resize(4);
	vertices[0].uv = { 0.0f,1.0f };
	vertices[1].uv = { 0.0f,0.0f };
	vertices[2].uv = { 1.0f,1.0f };
	vertices[3].uv = { 1.0f,0.0f };
	vertexBuffer->Create(vertices);

	// �萔�o�b�t�@
	constantBufferTransform->Init();
	constantBufferColor->Init();
	constantBufferCircleGauge->Init();
}

CircleGaugeSprite::~CircleGaugeSprite()
{
	delete vertexBuffer;
	delete constantBufferTransform;
	delete constantBufferColor;
	delete constantBufferCircleGauge;
}

void CircleGaugeSprite::Update()
{
	transform.pos = pos;
	transform.scale = { scale.x,scale.y,1 };
	transform.rot = { 0,0,rot };
	transform.Update();

	endRadian = Clamp(endRadian, 0, Radian(360));

	// �萔�o�b�t�@�ɓ]��
	constantBufferTransform->constantBufferMap->mat =
		transform.worldMat *
		Camera::current.GetOrthoGrphicProjectionMat();

	// �F�]��
	constantBufferColor->constantBufferMap->color = color / 255;
	constantBufferColor->constantBufferMap->color.a = color.a / 255;

	// �F�]��
	constantBufferCircleGauge->constantBufferMap->startRadian = startRadian;
	constantBufferCircleGauge->constantBufferMap->endRadian = endRadian + startRadian;

	// ���_�o�b�t�@�[�ɒ��_��]��
	TransferTexturePos();
}

void CircleGaugeSprite::Draw(const BlendMode& blendMode)
{
	SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer->GetvbViewAddress());

	// �}�e���A���ƃg�����X�t�H�[����CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferColor->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, constantBufferCircleGauge->constantBuffer->GetGPUVirtualAddress());

	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		renderBase->GetSpriteRootSignature()->GetRootDescriptorTableIndex(), texture->GetGpuHandle());

	renderBase->GetCommandList()->DrawInstanced((unsigned short)vertices.size(), 1, 0, 0);
}

void CircleGaugeSprite::SetBlendMode(const BlendMode& blendMode)
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

void CircleGaugeSprite::TransferTexturePos()
{
	// �V�����̃T�C�Y
	float width = texture->size.x;
	float height = texture->size.y;

	// ���݂̃T�C�Y
	float width2 = vertices[0].pos.x - vertices[2].pos.x;
	float height2 = vertices[1].pos.x - vertices[3].pos.x;

	if (width != fabsf(width2) || width != fabsf(height2))
	{
		vertices[0].pos = { (0.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //����
		vertices[1].pos = { (0.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //����
		vertices[2].pos = { (1.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //�E��
		vertices[3].pos = { (1.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //�E��

		vertexBuffer->TransferToBuffer(vertices);
	}
}