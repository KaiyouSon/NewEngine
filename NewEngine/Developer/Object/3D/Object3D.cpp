#include "Object3D.h"
#include "ViewProjection.h"
#include "RenderBase.h"
using namespace std;

Object3D::Object3D() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	isInitConstantBuffer(false), constantBuffer(new ConstantBuffer)
{
}
Object3D::~Object3D()
{
	delete constantBuffer;
}

void Object3D::Update()
{
	InitConstantBuffer();

	transform.pos = pos;
	transform.scale = scale;
	transform.rot = rot;
	transform.Update();

	// �萔�o�b�t�@�ɓ]��
	constantBuffer->constMapTransform->mat =
		transform.worldMat *
		view->matView *
		view->matProjection3D;

	constantBuffer->SetColor(color);
}
void Object3D::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance().get();

	renderBase->GetCommandList()->SetPipelineState(renderBase->GetBasicPipeline()->GetAlphaPipeline());
	renderBase->GetCommandList()->SetGraphicsRootSignature(renderBase->GetRootSignature());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, model.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(model.indexBuffer.GetibViewAddress());

	// �}�e���A���ƃg�����X�t�H�[����CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBuffer->GetConstBuffTransform()->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBuffer->GetConstBuffMaterial()->GetGPUVirtualAddress());

	// SRV�q�[�v�̐ݒ�R�}���h
	auto temp = renderBase->GetSrvDescHeap();
	renderBase->GetCommandList()->SetDescriptorHeaps(1, &temp);
	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	if (model.material.texture.isLoaded == false)
	{
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(2, texture.GetGpuHandle());
	}
	else
	{
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(2, model.material.texture.GetGpuHandle());
	}

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(unsigned short)model.indices.size(), 1, 0, 0, 0);
}

void Object3D::InitConstantBuffer()
{
	if (isInitConstantBuffer == true) return;

	// �萔�o�b�t�@������
	constantBuffer->TransformBufferInit();
	constantBuffer->MaterialBufferInit();
	isInitConstantBuffer = true;
}
