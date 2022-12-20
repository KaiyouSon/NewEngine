#include "Line.h"
#include "Camera.h"
#include "RenderBase.h"
#include <memory>
using namespace std;

Line::Line() :
	vertexBuffer(new VertexBuffer<VertexPosNormalUv>),
	indexBuffer(new IndexBuffer),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform3D>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>)
{
}

Line::~Line()
{
	delete vertexBuffer;
	delete indexBuffer;
	delete constantBufferTransform;
	delete constantBufferColor;
}

void Line::Initialize(const Vec3& startPos, const Vec3& endPos)
{
	//componentManager->GetComponent<Texture>()->SetTexture(*TextureBuffer::GetDefaultTexture());

	vertices.push_back({ startPos,{}, {0.0f, 1.0f} });	//左下
	vertices.push_back({ endPos,  {}, {0.0f, 1.0f} });	//左下

	indices.push_back(0);
	indices.push_back(1);

	vertexBuffer->Create(vertices);
	indexBuffer->Create(indices);

	// 定数バッファ
	constantBufferTransform->Init();
	constantBufferColor->Init();
}

void Line::Update()
{
	//componentManager->GetComponent<Transform>()->Update();

	//// 定数バッファに転送
	//constantBuffer->constMapTransform->mat =
	//	componentManager->GetComponent<Transform>()->GetWorldMat() *
	//	Camera::current.GetViewProjectionMat() *
	//	Camera::current.GetPerspectiveProjectionMat();

	//constantBuffer->SetColor(color);
}

void Line::Draw()
{
	//RenderBase* renderBase = RenderBase::GetInstance().get();

	//renderBase->GetCommandList()->SetPipelineState(renderBase->GetLinePipeline()->GetAlphaPipeline());
	//renderBase->GetCommandList()->SetGraphicsRootSignature(renderBase->GetRootSignature());
	//renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	//// VBVとIBVの設定コマンド
	//renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer->GetvbViewAddress());
	//renderBase->GetCommandList()->IASetIndexBuffer(indexBuffer->GetibViewAddress());

	//// マテリアルとトランスフォームのCBVの設定コマンド
	//renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
	//	0, constantBuffer->GetConstBuffMaterial()->GetGPUVirtualAddress());
	//renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
	//	1, constantBuffer->GetConstBuffTransform()->GetGPUVirtualAddress());

	//// SRVヒープの設定コマンド
	//auto temp = renderBase->GetSrvDescHeap();
	//renderBase->GetCommandList()->SetDescriptorHeaps(1, &temp);
	//// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	//renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
	//	2, componentManager->GetComponent<Texture>()->GetGpuHandle());

	//renderBase->GetCommandList()->DrawIndexedInstanced((unsigned short)indices.size(), 1, 0, 0, 0);
}
