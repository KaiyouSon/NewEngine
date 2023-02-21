#include "OutLineObj.h"
#include "RenderBase.h"

OutLineObj::OutLineObj() :
	color(Color::black),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform3D>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>)
{
	// 定数バッファ初期化
	constantBufferTransform->Init();	// 3D行列
	constantBufferColor->Init();		// 色
}

OutLineObj::~OutLineObj()
{
	delete constantBufferTransform;
	delete constantBufferColor;
}

void OutLineObj::Update(const OutLineObj* parent)
{
	obj->Update();

	transform.pos = obj->pos;
	transform.scale = obj->scale;
	transform.rot = obj->rot;
	transform.Update();
	if (parent != nullptr)
	{
		transform.worldMat *= parent->transform.worldMat;
	}

	// マトリックス転送
	constantBufferTransform->constantBufferMap->viewMat =
		Camera::current.GetViewLookToMat() *
		Camera::current.GetPerspectiveProjectionMat();
	constantBufferTransform->constantBufferMap->worldMat = transform.worldMat;
	constantBufferTransform->constantBufferMap->cameraPos = Camera::current.pos;

	// 色転送
	constantBufferColor->constantBufferMap->color = color / 255;
	constantBufferColor->constantBufferMap->color.a = color.a / 255;
}

void OutLineObj::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->SetPipelineState(
		GraphicsPipelineManager::GetGraphicsPipeline("Outline")->GetAlphaPipeline());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, obj->model->mesh.GetVertexBuffer().GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(obj->model->mesh.GetIndexBuffer().GetibViewAddress());

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferColor->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(unsigned short)obj->model->mesh.GetIndexSize(), 1, 0, 0, 0);

	obj->graphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("ToonRendering");
	obj->Draw(BlendMode::Alpha);
}
