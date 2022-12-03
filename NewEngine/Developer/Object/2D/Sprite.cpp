#include "Sprite.h"
#include "ViewProjection.h"
#include "MathUtil.h"
#include "RenderBase.h"
#include <memory>
using namespace std;

Sprite::Sprite() :
	vertexBuffer(new VertexBuffer<VertexPosUv>),
	indexBuffer(new IndexBuffer),
	constantBuffer(new ConstantBuffer)
{
	layer = false;
}

Sprite::~Sprite()
{
	delete vertexBuffer;
	delete indexBuffer;
	delete constantBuffer;
}

void Sprite::Initialize()
{
	//componentManager->GetComponent<Texture>()->SetTexture(*TextureBuffer::GetDefaultTexture());

	//float width = componentManager->GetComponent<Texture>()->GetTextureSize().x;
	//float height = componentManager->GetComponent<Texture>()->GetTextureSize().y;

	//vertices.push_back({ { -(width / 2), +(height / 2), 0.0f }, {0.0f, 1.0f} });	//左下
	//vertices.push_back({ { -(width / 2), -(height / 2), 0.0f }, {0.0f, 0.0f} });	//左上
	//vertices.push_back({ { +(width / 2), +(height / 2), 0.0f }, {1.0f, 1.0f} });	//右下
	//vertices.push_back({ { +(width / 2), -(height / 2), 0.0f }, {1.0f, 0.0f} });	//右上

	indices.push_back(0); indices.push_back(1); indices.push_back(2);	// 三角形1つ目
	indices.push_back(2); indices.push_back(1);	indices.push_back(3);	// 三角形2つ目

	vertexBuffer->Initialize(vertices);
	indexBuffer->Initialize(indices);

	// 定数バッファ
	constantBuffer->MaterialBufferInit();
	constantBuffer->TransformBufferInit();
}

void Sprite::Update()
{
	componentManager->GetComponent<Transform>()->Update();

	// 定数バッファに転送
	constantBuffer->constMapTransform->mat =
		componentManager->GetComponent<Transform>()->worldMat *
		view->matProjection2D;

	//float width = componentManager->GetComponent<Texture>()->GetTextureSize().x;
	//float height = componentManager->GetComponent<Texture>()->GetTextureSize().y;

	//if (width / 2 != mathUtil->Absolut(vertices.at(0).pos.x) ||
	//	height / 2 != mathUtil->Absolut(vertices.at(0).pos.y) ||
	//	width / 2 != mathUtil->Absolut(vertices.at(1).pos.x) ||
	//	height / 2 != mathUtil->Absolut(vertices.at(1).pos.y) ||
	//	width / 2 != mathUtil->Absolut(vertices.at(2).pos.x) ||
	//	height / 2 != mathUtil->Absolut(vertices.at(2).pos.y) ||
	//	width / 2 != mathUtil->Absolut(vertices.at(3).pos.x) ||
	//	height / 2 != mathUtil->Absolut(vertices.at(3).pos.y))
	//{
	//	vertices.at(0) = { { -(width / 2), +(height / 2), 0.0f }, {0.0f, 1.0f} };	// 左下
	//	vertices.at(1) = { { -(width / 2), -(height / 2), 0.0f }, {0.0f, 0.0f} };	//左上
	//	vertices.at(2) = { { +(width / 2), +(height / 2), 0.0f }, {1.0f, 1.0f} };	//右下
	//	vertices.at(3) = { { +(width / 2), -(height / 2), 0.0f }, {1.0f, 0.0f} };	//右上

	//	vertexBuffer->TransferToBuffer(vertices);
	//	vertexBuffer->Unmap();
	//}

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
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		2, componentManager->GetComponent<Texture>()->GetGpuHandle());

	renderBase->GetCommandList()->DrawIndexedInstanced((unsigned short)indices.size(), 1, 0, 0, 0);
}

bool Sprite::GetLayer()
{
	return layer;
}

Mat4 Sprite::GetFinalMat()
{
	return constantBuffer->constMapTransform->mat;
}