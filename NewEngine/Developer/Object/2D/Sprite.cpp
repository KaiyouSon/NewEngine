#include "Sprite.h"
#include "MathUtil.h"
#include "RenderBase.h"
#include <memory>
using namespace std;

Sprite::Sprite() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0), anchorPoint(0.0f, 0.0f),
	vertexBuffer(new VertexBuffer<VertexPosUv>),
	indexBuffer(new IndexBuffer),
	constantBuffer(new ConstantBuffer)
{
	vertices.push_back({ { 0.0f, 0.0f, 0.0f }, {0.0f, 1.0f} });	//左下
	vertices.push_back({ { 0.0f, 0.0f, 0.0f }, {0.0f, 0.0f} });	//左上
	vertices.push_back({ { 0.0f, 0.0f, 0.0f }, {1.0f, 1.0f} });	//右下
	vertices.push_back({ { 0.0f, 0.0f, 0.0f }, {1.0f, 0.0f} });	//右上

	indices.push_back(0); indices.push_back(1); indices.push_back(2);	// 三角形1つ目
	indices.push_back(2); indices.push_back(1);	indices.push_back(3);	// 三角形2つ目

	vertexBuffer->Initialize(vertices);
	indexBuffer->Initialize(indices);

	// 定数バッファ
	constantBuffer->MaterialBufferInit();
	constantBuffer->TransformBufferInit();
}

Sprite::~Sprite()
{
	delete vertexBuffer;
	delete indexBuffer;
	delete constantBuffer;
}

void Sprite::Update()
{
	transform.pos = pos;
	transform.scale = scale;
	transform.rot = rot;
	transform.Update();

	// 定数バッファに転送
	constantBuffer->constMapTransform->mat =
		transform.GetWorldMat() *
		Camera::current.GetOrthoGrphicProjectionMat();

	float width = texture.size.x;
	float height = texture.size.y;

	float width2 = vertices[0].pos.x - vertices[2].pos.x;
	float height2 = vertices[1].pos.x - vertices[3].pos.x;

	if (width != fabsf(width2) || width != fabsf(height2))
	{
		vertices[0].pos = { (0.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //左下
		vertices[1].pos = { (0.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //左上
		vertices[2].pos = { (1.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //右下
		vertices[3].pos = { (1.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //右上

		vertexBuffer->TransferToBuffer(vertices);
		vertexBuffer->Unmap();
	}

	constantBuffer->SetColor(color);
}

void Sprite::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance().get();

	renderBase->GetCommandList()->SetPipelineState(renderBase->GetSpritePipeline()->GetAlphaPipeline());
	renderBase->GetCommandList()->SetGraphicsRootSignature(renderBase->GetRootSignature());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer->GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(indexBuffer->GetibViewAddress());

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

	renderBase->GetCommandList()->DrawIndexedInstanced((unsigned short)indices.size(), 1, 0, 0, 0);
}