#include "Object3D.h"
#include "RenderBase.h"
#include "Light.h"
using namespace std;

Object3D::Object3D() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform3D>),
	constantBufferMaterial(new ConstantBuffer<ConstantBufferDataMaterial>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>)
{
	// 定数バッファ初期化
	constantBufferTransform->Init();	// 3D行列
	constantBufferMaterial->Init();		// マテリアル
	constantBufferColor->Init();		// 色

	texture.isMaterial = true;
}
Object3D::~Object3D()
{
	delete constantBufferTransform;
	delete constantBufferMaterial;
	delete constantBufferColor;
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

	// マトリックス転送
	constantBufferTransform->constantBufferMap->viewMat =
		Camera::current.GetViewProjectionMat() *
		Camera::current.GetPerspectiveProjectionMat();
	constantBufferTransform->constantBufferMap->worldMat = transform.GetWorldMat();
	constantBufferTransform->constantBufferMap->cameraPos = Camera::current.pos;

	// マテリアルの転送
	constantBufferMaterial->constantBufferMap->ambient = model.material.ambient;
	constantBufferMaterial->constantBufferMap->diffuse = model.material.diffuse;
	constantBufferMaterial->constantBufferMap->specular = model.material.specular;
	constantBufferMaterial->constantBufferMap->alpha = model.material.alpha;

	// 色転送
	constantBufferColor->constantBufferMap->color = color / 255;
}
void Object3D::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->SetGraphicsRootSignature(renderBase->GetRootSignature());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, model.mesh.GetVertexBuffer().GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(model.mesh.GetIndexBuffer().GetibViewAddress());

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferMaterial->constantBuffer->GetGPUVirtualAddress());
	Light::GetCurrent()->Draw();

	// SRVヒープの設定コマンド
	auto temp = renderBase->GetSrvDescHeap();
	renderBase->GetCommandList()->SetDescriptorHeaps(1, &temp);
	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(2, texture.GetGpuHandle());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(unsigned short)model.mesh.GetIndexSize(), 1, 0, 0, 0);
}

void Object3D::SetBlendMode(const BlendMode& blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	switch (blendMode)
	{
	case BlendMode::Alpha: // αブレンド
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetLoadModelPipeline()->GetAlphaPipeline());
		break;

	case BlendMode::Add:	// 加算ブレンド
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetLoadModelPipeline()->GetAddPipeline());
		break;

	case BlendMode::Sub:	// 減算ブレンド
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetLoadModelPipeline()->GetSubPipeline());
		break;

	case BlendMode::Inv:	// 反転
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetLoadModelPipeline()->GetInvPipeline());
		break;

	default:
		break;
	}
}

