#include "SilhouetteObj.h"
#include "RenderBase.h"
using namespace std;

SilhouetteObj::SilhouetteObj() :
	color(Color::black),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform3D>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>)
{
	// 定数バッファ初期化
	constantBufferTransform->Init();	// 3D行列
	constantBufferColor->Init();		// 色
}
SilhouetteObj::~SilhouetteObj()
{
	delete constantBufferTransform;
	delete constantBufferColor;
}
void SilhouetteObj::Update(const SilhouetteObj* parent)
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
void SilhouetteObj::Draw()
{
	SetBlendMode(BlendMode::Alpha);
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, obj->model.mesh.GetVertexBuffer().GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(obj->model.mesh.GetIndexBuffer().GetibViewAddress());

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferColor->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(unsigned short)obj->model.mesh.GetIndexSize(), 1, 0, 0, 0);

	obj->Draw(BlendMode::Alpha);
}

void SilhouetteObj::SetBlendMode(const BlendMode& blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	switch (blendMode)
	{
	case BlendMode::Alpha: // αブレンド
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetSilhouettePipeline()->GetAlphaPipeline());
		break;

	case BlendMode::Add:	// 加算ブレンド
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetSilhouettePipeline()->GetAddPipeline());
		break;

	case BlendMode::Sub:	// 減算ブレンド
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetSilhouettePipeline()->GetSubPipeline());
		break;

	case BlendMode::Inv:	// 反転
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetSilhouettePipeline()->GetInvPipeline());
		break;

	case BlendMode::Screen:	// 反転
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetSilhouettePipeline()->GetScreenPipeline());
		break;

	default:
		break;
	}
}

