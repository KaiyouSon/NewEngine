#include "Object3D.h"
#include "RenderBase.h"
#include "Light.h"
using namespace std;

Object3D::Object3D() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform3D>),
	constantBufferMaterial(new ConstantBuffer<ConstantBufferDataMaterial>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>)
{
	// �萔�o�b�t�@������
	constantBufferTransform->Init();	// 3D�s��
	constantBufferMaterial->Init();		// �}�e���A��
	constantBufferColor->Init();		// �F

	texture.isMaterial = true;
}
Object3D::~Object3D()
{
	delete constantBufferTransform;
	delete constantBufferMaterial;
	delete constantBufferColor;
}
void Object3D::Update()
{
	if (texture.isMaterial == true)
	{
		texture = model.material.texture;
	}

	transform.pos = pos;
	transform.scale = scale;
	transform.rot = rot;
	transform.Update();

	// �}�g���b�N�X�]��
	constantBufferTransform->constantBufferMap->viewMat =
		Camera::current.GetViewProjectionMat() *
		Camera::current.GetPerspectiveProjectionMat();
	constantBufferTransform->constantBufferMap->worldMat = transform.GetWorldMat();
	constantBufferTransform->constantBufferMap->cameraPos = Camera::current.pos;

	// �}�e���A���̓]��
	constantBufferMaterial->constantBufferMap->ambient = model.material.ambient;
	constantBufferMaterial->constantBufferMap->diffuse = model.material.diffuse;
	constantBufferMaterial->constantBufferMap->specular = model.material.specular;
	constantBufferMaterial->constantBufferMap->alpha = model.material.alpha;

	// �F�]��
	constantBufferColor->constantBufferMap->color = color / 255;
}
void Object3D::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->SetGraphicsRootSignature(renderBase->GetRootSignature());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, model.mesh.GetVertexBuffer().GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(model.mesh.GetIndexBuffer().GetibViewAddress());

	// �}�e���A���ƃg�����X�t�H�[����CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferMaterial->constantBuffer->GetGPUVirtualAddress());
	Light::GetCurrent()->Draw();

	// SRV�q�[�v�̐ݒ�R�}���h
	auto temp = renderBase->GetSrvDescHeap();
	renderBase->GetCommandList()->SetDescriptorHeaps(1, &temp);
	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(2, texture.GetGpuHandle());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(unsigned short)model.mesh.GetIndexSize(), 1, 0, 0, 0);
}

void Object3D::SetBlendMode(const BlendMode& blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	switch (blendMode)
	{
	case BlendMode::Alpha: // ���u�����h
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetLoadModelPipeline()->GetAlphaPipeline());
		break;

	case BlendMode::Add:	// ���Z�u�����h
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetLoadModelPipeline()->GetAddPipeline());
		break;

	case BlendMode::Sub:	// ���Z�u�����h
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetLoadModelPipeline()->GetSubPipeline());
		break;

	case BlendMode::Inv:	// ���]
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetLoadModelPipeline()->GetInvPipeline());
		break;

	default:
		break;
	}
}

