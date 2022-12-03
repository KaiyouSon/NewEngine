#include "Object3D.h"
#include "ViewProjection.h"
#include "RenderBase.h"
using namespace std;

Object3D::Object3D() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	isInitConstantBuffer(false), constantBuffer(new ConstantBuffer)
{
}
Object3D::~Object3D()
{
	delete constantBuffer;
}

void Object3D::Update()
{
	InitConstantBuffer();

	transform.pos = pos;
	transform.scale = scale;
	transform.rot = rot;
	transform.Update();

	// 定数バッファに転送
	constantBuffer->constMapTransform->mat =
		transform.worldMat *
		view->matView *
		view->matProjection3D;

	constantBuffer->SetColor(color);
}
void Object3D::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance().get();

	renderBase->GetCommandList()->SetPipelineState(renderBase->GetBasicPipeline()->GetAlphaPipeline());
	renderBase->GetCommandList()->SetGraphicsRootSignature(renderBase->GetRootSignature());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, model.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(model.indexBuffer.GetibViewAddress());

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBuffer->GetConstBuffTransform()->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBuffer->GetConstBuffMaterial()->GetGPUVirtualAddress());

	// SRVヒープの設定コマンド
	auto temp = renderBase->GetSrvDescHeap();
	renderBase->GetCommandList()->SetDescriptorHeaps(1, &temp);
	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	if (model.material.texture.isLoaded == false)
	{
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(2, texture.GetGpuHandle());
	}
	else
	{
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(2, model.material.texture.GetGpuHandle());
	}

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(unsigned short)model.indices.size(), 1, 0, 0, 0);
}

void Object3D::InitConstantBuffer()
{
	if (isInitConstantBuffer == true) return;

	// 定数バッファ初期化
	constantBuffer->TransformBufferInit();
	constantBuffer->MaterialBufferInit();
	isInitConstantBuffer = true;
}
