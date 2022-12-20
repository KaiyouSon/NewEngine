#include "Sprite.h"
#include "MathUtil.h"
#include "RenderBase.h"
#include <memory>
using namespace std;

Sprite::Sprite() :
	pos(0), scale(1), rot(0), anchorPoint(0.5f),
	vertexBuffer(new VertexBuffer<VertexPosUv>),
	//indexBuffer(new IndexBuffer),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform2D>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>)
{
	vertices.push_back({ { 0.0f, 0.0f, 0.0f }, {0.0f, 1.0f} });	//左下
	vertices.push_back({ { 0.0f, 0.0f, 0.0f }, {0.0f, 0.0f} });	//左上
	vertices.push_back({ { 0.0f, 0.0f, 0.0f }, {1.0f, 1.0f} });	//右下
	vertices.push_back({ { 0.0f, 0.0f, 0.0f }, {1.0f, 0.0f} });	//右上

	//indices.push_back(0); indices.push_back(1); indices.push_back(2);	// 三角形1つ目
	//indices.push_back(2); indices.push_back(1);	indices.push_back(3);	// 三角形2つ目

	vertexBuffer->Create(vertices);
	//indexBuffer->Create(indices);

	// 定数バッファ
	constantBufferTransform->Init();
	constantBufferColor->Init();
}

Sprite::~Sprite()
{
	delete vertexBuffer;
	//delete indexBuffer;
	delete constantBufferTransform;
	delete constantBufferColor;
}

void Sprite::Update()
{
	transform.pos = pos;
	transform.scale = scale;
	transform.rot = { 0,0,rot };
	transform.Update();

	// 定数バッファに転送
	constantBufferTransform->constantBufferMap->mat =
		transform.worldMat *
		Camera::current.GetOrthoGrphicProjectionMat();

	// 色転送
	constantBufferColor->constantBufferMap->color = color;

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

		//vertices[0].pos = { (0.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //左上
		//vertices[1].pos = { (0.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //左下
		//vertices[2].pos = { (1.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //右上
		//vertices[3].pos = { (1.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //右下

		vertexBuffer->TransferToBuffer(vertices);
	}
}

void Sprite::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->SetPipelineState(renderBase->GetSpritePipeline()->GetAlphaPipeline());
	renderBase->GetCommandList()->SetGraphicsRootSignature(renderBase->GetRootSignature());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer->GetvbViewAddress());
	//renderBase->GetCommandList()->IASetIndexBuffer(indexBuffer->GetibViewAddress());

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, constantBufferColor->constantBuffer->GetGPUVirtualAddress());

	// SRVヒープの設定コマンド
	auto temp = renderBase->GetSrvDescHeap();
	renderBase->GetCommandList()->SetDescriptorHeaps(1, &temp);
	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(0, texture.GetGpuHandle());

	//renderBase->GetCommandList()->DrawIndexedInstanced((unsigned short)indices.size(), 1, 0, 0, 0);
	renderBase->GetCommandList()->DrawInstanced((unsigned short)vertices.size(), 1, 0, 0);
}

void Sprite::SetBlendMode(const BlendMode& blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();//.get();

	switch (blendMode)
	{
	case BlendMode::Alpha: // αブレンド
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetSpritePipeline()->GetAlphaPipeline());
		break;

	case BlendMode::Add:	// 加算ブレンド
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetSpritePipeline()->GetAddPipeline());
		break;

	case BlendMode::Sub:	// 減算ブレンド
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetSpritePipeline()->GetSubPipeline());
		break;

	case BlendMode::Inv:	// 反転
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetSpritePipeline()->GetInvPipeline());
		break;

	default:
		break;
	}
}