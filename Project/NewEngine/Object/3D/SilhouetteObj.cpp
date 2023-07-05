#include "SilhouetteObj.h"
#include "RenderBase.h"
#include "Camera.h"
using namespace ConstantBufferData;

SilhouetteObj::SilhouetteObj() :
	color(Color::black),
	constantBufferTransform_(std::make_unique<ConstantBuffer<CTransform3D>>()),
	constantBufferColor_(std::make_unique<ConstantBuffer<CColor>>()),
	graphicsPipeline_(GraphicsPipelineManager::GetGraphicsPipeline("Silhouette"))
{
	// �萔�o�b�t�@������
	constantBufferTransform_->Create();	// 3D�s��
	constantBufferColor_->Create();		// �F
}

void SilhouetteObj::Update(Transform* parent)
{
	obj->Update();

	transform_.pos = obj->pos;
	transform_.scale = obj->scale;
	transform_.rot = obj->rot;
	transform_.Update();
	if (parent != nullptr)
	{
		parent_ = parent;

		Mat4 mat = transform_.GetWorldMat();
		mat *= parent_->GetWorldMat();
		transform_.SetWorldMat(mat);
	}

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
void SilhouetteObj::Draw(const BlendMode& blendMode)
{
	SetBlendMode(BlendMode::Alpha);
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, obj->GetModel()->mesh.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(obj->GetModel()->mesh.indexBuffer.GetibViewAddress());

	// �}�e���A���ƃg�����X�t�H�[����CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform_->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferColor_->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(uint16_t)obj->GetModel()->mesh.indices.size(), 1, 0, 0, 0);

	obj->Draw(BlendMode::Alpha);
}

void SilhouetteObj::SetBlendMode(const BlendMode& blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

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
