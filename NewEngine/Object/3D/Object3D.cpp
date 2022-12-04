#include "Object3D.h"
#include "RenderBase.h"
using namespace std;

Object3D::Object3D() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	constantBuffer(new ConstantBuffer)
{
	// 定数バッファ初期化
	constantBuffer->TransformBufferInit();
	constantBuffer->MaterialBufferInit();
	texture.isMaterial = true;

}
Object3D::~Object3D()
{
	delete constantBuffer;
}
void Object3D::Update()
{
	if (texture.isMaterial == true)
	{
		texture = model.material.texture;
	}

	transform.pos = pos;
	transform.scale = scale;
	transform.rot = rot;
	transform.Update();

	// 定数バッファに転送
	constantBuffer->constMapTransform->mat =
		transform.GetWorldMat() *
		Camera::current.GetViewProjectionMat() *
		Camera::current.GetPerspectiveProjectionMat();

	constantBuffer->SetColor(color);
}
void Object3D::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance().get();

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
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(2, texture.GetGpuHandle());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(unsigned short)model.indices.size(), 1, 0, 0, 0);
}

void Object3D::SetBlendMode(const BlendMode& blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance().get();

	switch (blendMode)
	{
	case BlendMode::Alpha: // αブレンド
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetBasicPipeline()->GetAlphaPipeline());
		break;

	case BlendMode::Add:	// 加算ブレンド
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetBasicPipeline()->GetAddPipeline());
		break;

	case BlendMode::Sub:	// 減算ブレンド
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetBasicPipeline()->GetSubPipeline());
		break;

	case BlendMode::Inv:	// 反転
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetBasicPipeline()->GetInvPipeline());
		break;

	default:
		break;
	}
}

