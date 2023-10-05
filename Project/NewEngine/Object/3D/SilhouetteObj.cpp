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
	// 定数バッファ初期化
	mConstantBufferTransform->Create();	// 3D行列
	mConstantBufferColor->Create();		// 色
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

	// マトリックス転送
	mConstantBufferTransform->constantBufferMap->viewMat =
		Camera::current.GetViewLookToMat() *
		Camera::current.GetPerspectiveProjectionMat();
	mConstantBufferTransform->constantBufferMap->worldMat = mTransform.GetWorldMat();
	mConstantBufferTransform->constantBufferMap->cameraPos = Camera::current.pos;

	// 色転送
	mConstantBufferColor->constantBufferMap->color = color / 255;
	mConstantBufferColor->constantBufferMap->color.a = color.a / 255;
}
void SilhouetteObj::Draw(const BlendMode& blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, obj->GetModel()->mesh.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(obj->GetModel()->mesh.indexBuffer.GetibViewAddress());

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, mConstantBufferTransform->bufferResource->buffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, mConstantBufferColor->bufferResource->buffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(uint16_t)obj->GetModel()->mesh.indices.size(), 1, 0, 0, 0);

	obj->Draw(BlendMode::Alpha);
}

