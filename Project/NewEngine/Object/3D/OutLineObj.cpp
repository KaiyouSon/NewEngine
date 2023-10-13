#include "OutLineObj.h"
#include "RenderBase.h"

using namespace ConstantBufferData;

OutLineObj::OutLineObj() :
	color(Color::black),
	mConstantBufferTransform(std::make_unique<ConstantBuffer<CTransform3D>>()),
	mConstantBufferColor(std::make_unique<ConstantBuffer<CColor>>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Outline"))
{
	// 螳壽焚繝舌ャ繝輔ぃ蛻晄悄蛹・
	mConstantBufferTransform->Create();	// 3D陦悟・
	mConstantBufferColor->Create();		// 濶ｲ
}

void OutLineObj::Update(Transform* parent)
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
	// 繝槭ヨ繝ｪ繝・け繧ｹ霆｢騾・
	mConstantBufferTransform->constantBufferMap->viewMat =
		Camera::current.GetViewLookToMat() *
		Camera::current.GetPerspectiveProjectionMat();
	mConstantBufferTransform->constantBufferMap->worldMat = mTransform.GetWorldMat();
	mConstantBufferTransform->constantBufferMap->cameraPos = Camera::current.pos;

	// 濶ｲ霆｢騾・
	mConstantBufferColor->constantBufferMap->color = color / 255;
	mConstantBufferColor->constantBufferMap->color.a = color.a / 255;
}

void OutLineObj::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline謠冗判繧ｳ繝槭Φ繝・
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBV縺ｨIBV縺ｮ險ｭ螳壹さ繝槭Φ繝・
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, obj->GetModel()->mesh.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(obj->GetModel()->mesh.indexBuffer.GetibViewAddress());

	// CBV縺ｮ險ｭ螳壹さ繝槭Φ繝・
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, mConstantBufferTransform->bufferResource->buffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, mConstantBufferColor->bufferResource->buffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(uint16_t)obj->GetModel()->mesh.indices.size(), 1, 0, 0, 0);

	obj->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ToonRendering"));
	obj->Draw(BlendMode::Alpha);
}
