#include "Line.h"
#include "Camera.h"
#include "RenderBase.h"
#include <memory>
using namespace std;

Line::Line() :
	vertexBuffer(new VertexBuffer<VertexPos>),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform3D>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>),
	graphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Line"))
{
	// 定数バッファ初期化
	constantBufferTransform->Init();	// 3D行列
	constantBufferColor->Init();		// 色

	vertices.resize(2);
	vertices[0] = { { 0.f,0.f,0.f} };
	vertices[1] = { { 1.f,0.f,0.f} };
	vertexBuffer->Create(vertices);
}

Line::~Line()
{
	delete vertexBuffer;
	delete constantBufferTransform;
	delete constantBufferColor;
}

void Line::Update()
{
	transform.pos = pos;
	transform.scale = { scale,1,1 };
	transform.rot = rot;
	transform.Update();

	// マトリックス転送
	constantBufferTransform->constantBufferMap->viewMat =
		Camera::current.GetViewLookToMat() *
		Camera::current.GetPerspectiveProjectionMat();
	constantBufferTransform->constantBufferMap->worldMat = transform.GetWorldMat();
	constantBufferTransform->constantBufferMap->cameraPos = Camera::current.pos;

	// 色転送
	constantBufferColor->constantBufferMap->color = color / 255;
	constantBufferColor->constantBufferMap->color.a = color.a / 255;
}

void Line::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();

	renderBase->GetCommandList()->SetPipelineState(graphicsPipeline->GetAlphaPipeline());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer->GetvbViewAddress());

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, constantBufferColor->constantBuffer->GetGPUVirtualAddress());

	// 描画コマンド
	renderBase->GetCommandList()->DrawInstanced((UINT)vertices.size(), 1, 0, 0);
}
