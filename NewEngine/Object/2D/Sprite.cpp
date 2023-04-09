#include "Sprite.h"
#include "MathUtil.h"
#include "RenderBase.h"
#include "TextureManager.h"
#include "Camera.h"
using namespace std;

Sprite::Sprite() :
	texture(TextureManager::GetTexture("White")),
	pos(0), scale(1), size(0), rot(0), color(Color::white), anchorPoint(0.5f),
	vertexBuffer(new VertexBuffer<VertexPosUv>),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform2D>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>),
	graphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Sprite"))
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
}

Sprite::~Sprite()
{
	delete vertexBuffer;
	delete constantBufferTransform;
	delete constantBufferColor;
}

void Sprite::Update()
{
	transform.pos = pos;
	transform.scale = { scale.x,scale.y,1 };
	transform.rot = { 0,0,rot };
	transform.Update();

	// �萔�o�b�t�@�ɓ]��
	constantBufferTransform->constantBufferMap->mat =
		transform.worldMat *
		Camera::current.GetOrthoGrphicProjectionMat();

	// �F�]��
	constantBufferColor->constantBufferMap->color = color / 255;
	constantBufferColor->constantBufferMap->color.a = color.a / 255;

	// ���_�o�b�t�@�[�ɒ��_��]��
	TransferTexturePos(size);
}

void Sprite::Draw(const BlendMode& blendMode)
{
	SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	//renderBase->GetCommandList()->SetPipelineState(renderBase->GetSpritePipeline()->GetAlphaPipeline());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer->GetvbViewAddress());

	// �}�e���A���ƃg�����X�t�H�[����CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferColor->constantBuffer->GetGPUVirtualAddress());

	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		renderBase->GetSpriteRootSignature()->GetRootDescriptorTableIndex(), texture->GetGpuHandle());

	renderBase->GetCommandList()->DrawInstanced((unsigned short)vertices.size(), 1, 0, 0);
}

void Sprite::SetBlendMode(const BlendMode& blendMode)
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

void Sprite::SetTextureRect(const Vec2& leftTopPos, const Vec2 rightDownPos)
{
	float left = leftTopPos.x / texture->buffer->GetDesc().Width;
	float right = rightDownPos.x / texture->buffer->GetDesc().Width;
	float up = leftTopPos.y / texture->buffer->GetDesc().Height;
	float down = rightDownPos.y / texture->buffer->GetDesc().Height;

	vertices[0].uv = { left,down };
	vertices[1].uv = { left,up };
	vertices[2].uv = { right,down };
	vertices[3].uv = { right,up };

	vertexBuffer->TransferToBuffer(vertices);
}