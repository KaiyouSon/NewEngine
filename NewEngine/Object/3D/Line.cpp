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

	vertices.push_back({ startPos,{}, {0.0f, 1.0f} });	//����
	vertices.push_back({ endPos,  {}, {0.0f, 1.0f} });	//����

	indices.push_back(0);
	indices.push_back(1);

	vertexBuffer->Create(vertices);
	indexBuffer->Create(indices);

	// �萔�o�b�t�@
	constantBufferTransform->Init();
	constantBufferColor->Init();
}

void Line::Update()
{
	//componentManager->GetComponent<Transform>()->Update();

	//// �萔�o�b�t�@�ɓ]��
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

	//// VBV��IBV�̐ݒ�R�}���h
	//renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer->GetvbViewAddress());
	//renderBase->GetCommandList()->IASetIndexBuffer(indexBuffer->GetibViewAddress());

	//// �}�e���A���ƃg�����X�t�H�[����CBV�̐ݒ�R�}���h
	//renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
	//	0, constantBuffer->GetConstBuffMaterial()->GetGPUVirtualAddress());
	//renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
	//	1, constantBuffer->GetConstBuffTransform()->GetGPUVirtualAddress());

	//// SRV�q�[�v�̐ݒ�R�}���h
	//auto temp = renderBase->GetSrvDescHeap();
	//renderBase->GetCommandList()->SetDescriptorHeaps(1, &temp);
	//// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	//renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
	//	2, componentManager->GetComponent<Texture>()->GetGpuHandle());

	//renderBase->GetCommandList()->DrawIndexedInstanced((unsigned short)indices.size(), 1, 0, 0, 0);
}
