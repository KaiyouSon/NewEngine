#include "Object3D.h"
#include "ViewProjection.h"
#include "RenderBase.h"
#include <memory>
using namespace std;

Object3D::Object3D() :
	vertexBuffer(new VertexBuffer<VertexPosNormalUv>),
	indexBuffer(new IndexBuffer),
	constantBuffer(new ConstantBuffer)
{
}

Object3D::~Object3D()
{
	delete vertexBuffer;
	delete indexBuffer;
	delete constantBuffer;
}

void Object3D::Initialize()
{
	// 頂点バッファ
	vertexBuffer->Initialize(componentManager->GetComponent<ModelData>()->vertices);
	// インデックスバッファ
	indexBuffer->Initialize(componentManager->GetComponent<ModelData>()->indices);

	// 定数バッファ
	constantBuffer->TransformBufferInit();
	//constantBuffer->MaterialBufferInit();
	//componentManager->GetComponent<Texture>()->
	//	SetTexture(*TextureBuffer::GetDefaultTexture());

	if (componentManager->GetComponent<ModelData>()->GetDataType() == DefaultModel)
	{
		constantBuffer->MaterialBufferInit();

		componentManager->GetComponent<Texture>()->
			SetTexture(*TextureBuffer::GetDefaultTexture());
	}
	else
	{
		constantBuffer->MaterialBufferInit(*materialList->GetMaterial(
			componentManager->GetComponent<ModelData>()->GetIndex()));

		componentManager->GetComponent<Texture>()->SetTexture(*materialTextureList->GetTexture(
			componentManager->GetComponent<ModelData>()->GetIndex()));
	}

	// テクスチャー

}

void Object3D::Update()
{
	ColliderManagerSetting();

	componentManager->GetComponent<Transform>()->Update();

	// 定数バッファに転送
	constantBuffer->constMapTransform->mat =
		componentManager->GetComponent<Transform>()->worldMat *
		view->matView *
		view->matProjection3D;
	if (componentManager->GetComponent<ModelData>()->GetDataType() == DefaultModel)
	{
		constantBuffer->SetColor(color);
	}
	//static Dirty<Color> colorDirty(color);
	//if (colorDirty.GetisDirty(color) == true) 
}

void Object3D::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance().get();

	if (componentManager->GetComponent<ModelData>()->GetDataType() == DefaultModel)
	{
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetBasicPipeline()->GetAlphaPipeline());
		renderBase->GetCommandList()->SetGraphicsRootSignature(renderBase->GetRootSignature());
		renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	else
	{
		renderBase->GetCommandList()->SetPipelineState(renderBase->GetLoadModelPipeline()->GetAlphaPipeline());
		renderBase->GetCommandList()->SetGraphicsRootSignature(renderBase->GetRootSignature());
		renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

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

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(unsigned short)componentManager->GetComponent<ModelData>()->indices.size(), 1, 0, 0, 0);
}

Mat4 Object3D::GetFinalMat()
{
	return constantBuffer->constMapTransform->mat;
}
