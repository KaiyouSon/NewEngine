#include "OutLineObj.h"
#include "RenderBase.h"

OutLineObj::OutLineObj() :
	color(Color::black),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform3D>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>)
{
	// �萔�o�b�t�@������
	constantBufferTransform->Init();	// 3D�s��
	constantBufferColor->Init();		// �F
}

OutLineObj::~OutLineObj()
{
	delete constantBufferTransform;
	delete constantBufferColor;
}

void OutLineObj::Update(OutLineObj* parent)
{
	obj->Update();

	transform.pos = obj->pos_;
	transform.scale = obj->scale_;
	transform.rot = obj->rot_;
	transform.Update();
	if (parent != nullptr)
	{
		transform.GetWorldMat() *= parent->transform.GetWorldMat();
	}

	// �}�g���b�N�X�]��
	constantBufferTransform->constantBufferMap->viewMat =
		Camera::current.GetViewLookToMat() *
		Camera::current.GetPerspectiveProjectionMat();
	constantBufferTransform->constantBufferMap->worldMat = transform.GetWorldMat();
	constantBufferTransform->constantBufferMap->cameraPos = Camera::current.pos;

	// �F�]��
	constantBufferColor->constantBufferMap->color = color / 255;
	constantBufferColor->constantBufferMap->color.a = color.a / 255;
}

void OutLineObj::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->SetPipelineState(
		GraphicsPipelineManager::GetGraphicsPipeline("Outline")->GetAlphaPipeline());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, obj->GetModel()->mesh.GetVertexBuffer().GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(obj->GetModel()->mesh.GetIndexBuffer().GetibViewAddress());

	// CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferColor->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(unsigned short)obj->GetModel()->mesh.GetIndexSize(), 1, 0, 0, 0);

	obj->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("ToonRendering"));
	obj->Draw(BlendMode::Alpha);
}
