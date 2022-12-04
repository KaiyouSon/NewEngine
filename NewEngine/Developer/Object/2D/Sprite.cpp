#include "Sprite.h"
#include "MathUtil.h"
#include "RenderBase.h"
#include <memory>
using namespace std;

Sprite::Sprite() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0), anchorPoint(0.0f, 0.0f),
	vertexBuffer(new VertexBuffer<VertexPosUv>),
	indexBuffer(new IndexBuffer),
	constantBuffer(new ConstantBuffer)
{
	vertices.push_back({ { 0.0f, 0.0f, 0.0f }, {0.0f, 1.0f} });	//����
	vertices.push_back({ { 0.0f, 0.0f, 0.0f }, {0.0f, 0.0f} });	//����
	vertices.push_back({ { 0.0f, 0.0f, 0.0f }, {1.0f, 1.0f} });	//�E��
	vertices.push_back({ { 0.0f, 0.0f, 0.0f }, {1.0f, 0.0f} });	//�E��

	indices.push_back(0); indices.push_back(1); indices.push_back(2);	// �O�p�`1��
	indices.push_back(2); indices.push_back(1);	indices.push_back(3);	// �O�p�`2��

	vertexBuffer->Initialize(vertices);
	indexBuffer->Initialize(indices);

	// �萔�o�b�t�@
	constantBuffer->MaterialBufferInit();
	constantBuffer->TransformBufferInit();
}

Sprite::~Sprite()
{
	delete vertexBuffer;
	delete indexBuffer;
	delete constantBuffer;
}

void Sprite::Update()
{
	transform.pos = pos;
	transform.scale = scale;
	transform.rot = rot;
	transform.Update();

	// �萔�o�b�t�@�ɓ]��
	constantBuffer->constMapTransform->mat =
		transform.GetWorldMat() *
		Camera::current.GetOrthoGrphicProjectionMat();

	float width = texture.size.x;
	float height = texture.size.y;

	float width2 = vertices[0].pos.x - vertices[2].pos.x;
	float height2 = vertices[1].pos.x - vertices[3].pos.x;

	if (width != fabsf(width2) || width != fabsf(height2))
	{
		vertices[0].pos = { (0.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //����
		vertices[1].pos = { (0.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //����
		vertices[2].pos = { (1.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //�E��
		vertices[3].pos = { (1.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //�E��

		vertexBuffer->TransferToBuffer(vertices);
		vertexBuffer->Unmap();
	}

	constantBuffer->SetColor(color);
}

void Sprite::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance().get();

	renderBase->GetCommandList()->SetPipelineState(renderBase->GetSpritePipeline()->GetAlphaPipeline());
	renderBase->GetCommandList()->SetGraphicsRootSignature(renderBase->GetRootSignature());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer->GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(indexBuffer->GetibViewAddress());

	// �}�e���A���ƃg�����X�t�H�[����CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBuffer->GetConstBuffTransform()->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBuffer->GetConstBuffMaterial()->GetGPUVirtualAddress());

	// SRV�q�[�v�̐ݒ�R�}���h
	auto temp = renderBase->GetSrvDescHeap();
	renderBase->GetCommandList()->SetDescriptorHeaps(1, &temp);
	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(2, texture.GetGpuHandle());

	renderBase->GetCommandList()->DrawIndexedInstanced((unsigned short)indices.size(), 1, 0, 0, 0);
}