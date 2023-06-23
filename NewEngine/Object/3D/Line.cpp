#include "Line.h"
#include "Camera.h"
#include "RenderBase.h"

using namespace VertexBufferData;
using namespace ConstantBufferData;

Line::Line() :
	vertexBuffer_(std::make_unique<VertexBuffer<VLine>>()),
	constantBufferTransform_(std::make_unique<ConstantBuffer<CTransform3D>>()),
	constantBufferColor_(std::make_unique<ConstantBuffer<CColor>>()),
	graphicsPipeline_(GraphicsPipelineManager::GetGraphicsPipeline("Line"))
{
	// 定数バッファ初期化
	constantBufferTransform_->Create();	// 3D行列
	constantBufferColor_->Create();		// 色

	vertices_.resize(2);
	vertices_[0] = { { 0.f,0.f,0.f} };
	vertices_[1] = { { 1.f,0.f,0.f} };
	vertexBuffer_->Create(vertices_);
}

void Line::Update()
{
	transform_.pos = pos;
	transform_.scale = { scale,1,1 };
	transform_.rot = rot;
	transform_.Update();

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

void Line::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();

	renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetAlphaPipeline());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer_->GetvbViewAddress());

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform_->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, constantBufferColor_->constantBuffer->GetGPUVirtualAddress());

	// 描画コマンド
	renderBase->GetCommandList()->DrawInstanced((UINT)vertices_.size(), 1, 0, 0);
}
