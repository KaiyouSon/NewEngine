#include "OutLineObj.h"
#include "RenderBase.h"

using namespace ConstantBufferData;

OutLineObj::OutLineObj() :
	color(Color::black),
	constantBufferTransform_(new ConstantBuffer<CTransform3D>),
	constantBufferColor_(new ConstantBuffer<CColor>)
{
	// 定数バッファ初期化
	constantBufferTransform_->Init();	// 3D行列
	constantBufferColor_->Init();		// 色
}

void OutLineObj::Update(OutLineObj* parent)
{
	obj->Update();

	transform_.pos = obj->pos;
	transform_.scale = obj->scale;
	transform_.rot = obj->rot;
	transform_.Update();
	if (parent != nullptr)
	{
		transform_.GetWorldMat() *= parent->transform_.GetWorldMat();
	}

	// マトリックス転送
	constantBufferTransform_->constantBufferMap->viewMat =
		Camera::current.GetViewLookToMat() *
		Camera::current.GetPerspectiveProjectionMat();
	constantBufferTransform_->constantBufferMap->worldMat = transform_.GetWorldMat();
	constantBufferTransform_->constantBufferMap->cameraPos = Camera::current.pos;

	// 色転送
	constantBufferColor_->constantBufferMap->color = color / 255;
	constantBufferColor_->constantBufferMap->color.a = color.a / 255;
}

void OutLineObj::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->SetPipelineState(
		GraphicsPipelineManager::GetGraphicsPipeline("Outline")->GetAlphaPipeline());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, obj->GetModel()->mesh.GetVertexBuffer().GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(obj->GetModel()->mesh.GetIndexBuffer().GetibViewAddress());

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform_->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferColor_->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(unsigned short)obj->GetModel()->mesh.GetIndexSize(), 1, 0, 0, 0);

	obj->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("ToonRendering"));
	obj->Draw(BlendMode::Alpha);
}
