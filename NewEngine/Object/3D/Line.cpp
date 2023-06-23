#include "Line.h"
#include "Camera.h"
#include "RenderBase.h"

using namespace VertexBufferData;
using namespace ConstantBufferData;

Line::Line() :
	vertexBuffer_(std::make_unique<VertexBuffer<VLine>>()),
	constantBufferTransform_(std::make_unique<ConstantBuffer<CTransform3D>>()),
	constantBufferColor_(std::make_unique<ConstantBuffer<CColor>>()),
	graphicsPipeline_(GraphicsPipelineManager::GetGraphicsPipeline("Line"))
{
	// �萔�o�b�t�@������
	constantBufferTransform_->Create();	// 3D�s��
	constantBufferColor_->Create();		// �F

	vertices_.resize(2);
	vertices_[0] = { { 0.f,0.f,0.f} };
	vertices_[1] = { { 1.f,0.f,0.f} };
	vertexBuffer_->Create(vertices_);
}

void Line::Update()
{
	transform_.pos = pos;
	transform_.scale = { scale,1,1 };
	transform_.rot = rot;
	transform_.Update();

	// �}�g���b�N�X�]��
	constantBufferTransform_->constantBufferMap->viewMat =
		Camera::current.GetViewLookToMat() *
		Camera::current.GetPerspectiveProjectionMat();
	constantBufferTransform_->constantBufferMap->worldMat = transform_.GetWorldMat();
	constantBufferTransform_->constantBufferMap->cameraPos = Camera::current.pos;

	// �F�]��
	constantBufferColor_->constantBufferMap->color = color / 255;
	constantBufferColor_->constantBufferMap->color.a = color.a / 255;
}

void Line::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();

	renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetAlphaPipeline());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer_->GetvbViewAddress());

	// CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform_->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, constantBufferColor_->constantBuffer->GetGPUVirtualAddress());

	// �`��R�}���h
	renderBase->GetCommandList()->DrawInstanced((UINT)vertices_.size(), 1, 0, 0);
}
