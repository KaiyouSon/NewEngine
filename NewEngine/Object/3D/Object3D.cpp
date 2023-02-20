#include "Object3D.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
using namespace std;

bool Object3D::isAllLighting = false;

Object3D::Object3D() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform3D>),
	constantBufferMaterial(new ConstantBuffer<ConstantBufferDataMaterial>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>),
	graphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Object3D")),
	isLighting(false)
{
	// 定数バッファ初期化
	constantBufferTransform->Init();	// 3D行列
	constantBufferMaterial->Init();		// マテリアル
	constantBufferColor->Init();		// 色

	texture.isMaterial = true;

	if (isAllLighting == true)
	{
		isLighting = true;
	}
}
Object3D::~Object3D()
{
	delete constantBufferTransform;
	delete constantBufferMaterial;
	delete constantBufferColor;
}
void Object3D::Update(const Object3D* parent)
{
	if (texture.isMaterial == true)
	{
		texture = model.material.texture;
	}

	transform.pos = pos;
	transform.scale = scale;
	transform.rot = rot;
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

	// マテリアルの転送
	if (isLighting == true && isAllLighting == true)
	{
		constantBufferMaterial->constantBufferMap->ambient = Vec3::one - 0.5f;
		//constantBufferMaterial->constantBufferMap->ambient = model.material.ambient;

		constantBufferMaterial->constantBufferMap->diffuse = model.material.diffuse;
		constantBufferMaterial->constantBufferMap->specular = model.material.specular;
	}
	else
	{
		constantBufferMaterial->constantBufferMap->ambient = Vec3::one;
		constantBufferMaterial->constantBufferMap->diffuse = Vec3::zero;
		constantBufferMaterial->constantBufferMap->specular = Vec3::zero;
	}
	constantBufferMaterial->constantBufferMap->alpha = model.material.alpha;

	// 色転送
	constantBufferColor->constantBufferMap->color = color / 255;
	constantBufferColor->constantBufferMap->color.a = color.a / 255;
}
void Object3D::Draw(const BlendMode& blendMode)
{
	SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, model.mesh.GetVertexBuffer().GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(model.mesh.GetIndexBuffer().GetibViewAddress());

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferMaterial->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, constantBufferColor->constantBuffer->GetGPUVirtualAddress());
	LightManager::GetInstance()->Draw();

	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		renderBase->GetObject3DRootSignature()->GetRootDescriptorTableIndex(), texture.GetGpuHandle());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(unsigned short)model.mesh.GetIndexSize(), 1, 0, 0, 0);
}

void Object3D::SetBlendMode(const BlendMode& blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	switch (blendMode)
	{
	case BlendMode::Alpha: // αブレンド
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline->GetAlphaPipeline());
		break;

	case BlendMode::Add:	// 加算ブレンド
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline->GetAddPipeline());
		break;

	case BlendMode::Sub:	// 減算ブレンド
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline->GetSubPipeline());
		break;

	case BlendMode::Inv:	// 反転
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline->GetInvPipeline());
		break;

	case BlendMode::Screen:	// 反転
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline->GetScreenPipeline());
		break;

	default:
		break;
	}
}

