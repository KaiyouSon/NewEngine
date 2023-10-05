#include "SilhouetteObj.h"
#include "RenderBase.h"
#include "Camera.h"
using namespace ConstantBufferData;

SilhouetteObj::SilhouetteObj() :
	color(Color::black),
	mConstantBufferTransform(std::make_unique<ConstantBuffer<CTransform3D>>()),
	mConstantBufferColor(std::make_unique<ConstantBuffer<CColor>>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Silhouette"))
{
	// �萔�o�b�t�@������
	mConstantBufferTransform->Create();	// 3D�s��
	mConstantBufferColor->Create();		// �F
}

void SilhouetteObj::Update(Transform* parent)
{
	obj->Update();

	mTransform.pos = obj->pos;
	mTransform.scale = obj->scale;
	mTransform.rot = obj->rot;
	mTransform.Update();
	if (parent != nullptr)
	{
		mParent = parent;

		Mat4 mat = mTransform.GetWorldMat();
		mat *= mParent->GetWorldMat();
		mTransform.SetWorldMat(mat);
	}

	// �}�g���b�N�X�]��
	mConstantBufferTransform->constantBufferMap->viewMat =
		Camera::current.GetViewLookToMat() *
		Camera::current.GetPerspectiveProjectionMat();
	mConstantBufferTransform->constantBufferMap->worldMat = mTransform.GetWorldMat();
	mConstantBufferTransform->constantBufferMap->cameraPos = Camera::current.pos;

	// �F�]��
	mConstantBufferColor->constantBufferMap->color = color / 255;
	mConstantBufferColor->constantBufferMap->color.a = color.a / 255;
}
void SilhouetteObj::Draw(const BlendMode& blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline�`��R�}���h
	mGraphicsPipeline->DrawCommand(blendMode);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, obj->GetModel()->mesh.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(obj->GetModel()->mesh.indexBuffer.GetibViewAddress());

	// �}�e���A���ƃg�����X�t�H�[����CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, mConstantBufferTransform->bufferResource->buffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, mConstantBufferColor->bufferResource->buffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(uint16_t)obj->GetModel()->mesh.indices.size(), 1, 0, 0, 0);

	obj->Draw(BlendMode::Alpha);
}

