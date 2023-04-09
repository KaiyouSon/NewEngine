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
	// �萔�o�b�t�@������
	constantBufferTransform->Init();	// 3D�s��
	constantBufferColor->Init();		// �F

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

	// �}�g���b�N�X�]��
	constantBufferTransform->constantBufferMap->viewMat =
		Camera::current.GetViewLookToMat() *
		Camera::current.GetPerspectiveProjectionMat();
	constantBufferTransform->constantBufferMap->worldMat = transform.worldMat;
	constantBufferTransform->constantBufferMap->cameraPos = Camera::current.pos;

	// �F�]��
	constantBufferColor->constantBufferMap->color = color / 255;
	constantBufferColor->constantBufferMap->color.a = color.a / 255;
}

void Line::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();

	renderBase->GetCommandList()->SetPipelineState(graphicsPipeline->GetAlphaPipeline());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer->GetvbViewAddress());

	// CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, constantBufferColor->constantBuffer->GetGPUVirtualAddress());

	// �`��R�}���h
	renderBase->GetCommandList()->DrawInstanced((UINT)vertices.size(), 1, 0, 0);
}
