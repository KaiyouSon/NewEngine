#include "SilhouetteObj.h"
#include "RenderBase.h"
#include "Camera.h"
using namespace ConstantBufferData;

SilhouetteObj::SilhouetteObj() :
	color(Color::black),
	mConstantBufferTransform(std::make_unique<ConstantBuffer<CTransform3D>>()),
	mConstantBufferColor(std::make_unique<ConstantBuffer<CColor>>()),
	mGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Silhouette"))
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
	SetBlendMode(BlendMode::Alpha);
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, obj->GetModel()->mesh.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(obj->GetModel()->mesh.indexBuffer.GetibViewAddress());

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, mConstantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, mConstantBufferColor->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(uint16_t)obj->GetModel()->mesh.indices.size(), 1, 0, 0, 0);

	obj->Draw(BlendMode::Alpha);
}

void SilhouetteObj::SetBlendMode(const BlendMode& blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	switch (blendMode)
	{
	case BlendMode::Alpha: // αブレンド
		renderBase->GetCommandList()->SetPipelineState(mGraphicsPipeline->GetAlphaPipeline());
		break;

	case BlendMode::Add:	// 加算ブレンド
		renderBase->GetCommandList()->SetPipelineState(mGraphicsPipeline->GetAddPipeline());
		break;

	case BlendMode::Sub:	// 減算ブレンド
		renderBase->GetCommandList()->SetPipelineState(mGraphicsPipeline->GetSubPipeline());
		break;

	case BlendMode::Inv:	// 反転
		renderBase->GetCommandList()->SetPipelineState(mGraphicsPipeline->GetInvPipeline());
		break;

	default:
		break;
	}
}

