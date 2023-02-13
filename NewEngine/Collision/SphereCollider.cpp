#include "SphereCollider.h"
#include "Camera.h"
#include "ModelManager.h"
#include "TextureManager.h"

SphereCollider::SphereCollider() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0), centerPos(pos), radius(scale.x),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform3D>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>),
	model(*ModelManager::GetModel("SphereCollider"))
{
	// �萔�o�b�t�@������
	constantBufferTransform->Init();	// 3D�s��
	constantBufferColor->Init();		// �F
}

SphereCollider::SphereCollider(const Vec3& centerPos, const float& radius) :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0), centerPos(centerPos), radius(radius),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform3D>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>),
	model(*ModelManager::GetModel("SphereCollider"))
{
	// �萔�o�b�t�@������
	constantBufferTransform->Init();	// 3D�s��
	constantBufferColor->Init();		// �F
}

SphereCollider::~SphereCollider()
{
	delete constantBufferTransform;
	delete constantBufferColor;
}

void SphereCollider::Update()
{
	transform.pos = centerPos;
	transform.scale = radius;
	transform.rot = Vec3::zero;
	transform.Update();

	// �}�g���b�N�X�]��
	constantBufferTransform->constantBufferMap->viewMat =
		Camera::current.GetViewLookToMat() *
		Camera::current.GetPerspectiveProjectionMat();
	constantBufferTransform->constantBufferMap->worldMat = transform.worldMat;
	constantBufferTransform->constantBufferMap->cameraPos = Camera::current.pos;

	// �F�]��
	constantBufferColor->constantBufferMap->color = color / 255;
	constantBufferColor->constantBufferMap->color.a = color.a / 255;
}

void SphereCollider::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->SetPipelineState(
		GraphicsPipelineManager::GetGraphicsPipeline("Collider")->GetAlphaPipeline());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, model.mesh.GetVertexBuffer().GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(model.mesh.GetIndexBuffer().GetibViewAddress());

	// CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, constantBufferColor->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(unsigned short)model.mesh.GetIndexSize(), 1, 0, 0, 0);
}