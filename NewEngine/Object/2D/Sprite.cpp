#include "Sprite.h"
#include "MathUtil.h"
#include "RenderBase.h"
#include <memory>
using namespace std;

Sprite::Sprite() :
	pos(0), scale(1), rot(0), anchorPoint(0.5f),
	vertexBuffer(new VertexBuffer<VertexPosUv>),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform2D>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>)
{
	vertices.resize(4);
	vertices[0].uv = { 0.0f,1.0f };
	vertices[1].uv = { 0.0f,0.0f };
	vertices[2].uv = { 1.0f,1.0f };
	vertices[3].uv = { 1.0f,0.0f };
	vertexBuffer->Create(vertices);

	// 定数バッファ
	constantBufferTransform->Init();
	constantBufferColor->Init();
}

Sprite::~Sprite()
{
	delete vertexBuffer;
	delete constantBufferTransform;
	delete constantBufferColor;
}

void Sprite::Update()
{
	transform.pos = pos;
	transform.scale = { scale.x,scale.y,1 };
	transform.rot = { 0,0,rot };
	transform.Update();

	// 定数バッファに転送
	constantBufferTransform->constantBufferMap->mat =
		transform.worldMat *
		Camera::current.GetOrthoGrphicProjectionMat();

	// 色転送
	constantBufferColor->constantBufferMap->color = color / 255;
	constantBufferColor->constantBufferMap->color.a = color.a / 255;

	// 頂点バッファーに頂点を転送
	TransferTexturePos();
}

void Sprite::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->SetPipelineState(renderBase->GetSpritePipeline()->GetAlphaPipeline());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer->GetvbViewAddress());

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferColor->constantBuffer->GetGPUVirtualAddress());

	// SRVヒープの設定コマンド
	auto temp = renderBase->GetSrvDescHeap();
	renderBase->GetCommandList()->SetDescriptorHeaps(1, &temp);
	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		renderBase->GetSpriteRootSignature()->GetRootDescriptorTableIndex(), texture.GetGpuHandle());

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

void Sprite::TransferTexturePos()
{
	// 新しいのサイズ
	float width = texture.size.x;
	float height = texture.size.y;

	// 現在のサイズ
	float width2 = vertices[0].pos.x - vertices[2].pos.x;
	float height2 = vertices[1].pos.x - vertices[3].pos.x;

	if (width != fabsf(width2) || width != fabsf(height2))
	{
		vertices[0].pos = { (0.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //左下
		vertices[1].pos = { (0.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //左上
		vertices[2].pos = { (1.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //右下
		vertices[3].pos = { (1.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //右上

		vertexBuffer->TransferToBuffer(vertices);
	}
}

void Sprite::SetTextureRect(const Vec2& leftTopPos, const Vec2 rightDownPos)
{
	float left = leftTopPos.x / texture.buffer->GetDesc().Width;
	float right = rightDownPos.x / texture.buffer->GetDesc().Width;
	float up = leftTopPos.y / texture.buffer->GetDesc().Height;
	float down = rightDownPos.y / texture.buffer->GetDesc().Height;

	vertices[0].uv = { left,down };
	vertices[1].uv = { left,up };
	vertices[2].uv = { right,down };
	vertices[3].uv = { right,up };

	vertexBuffer->TransferToBuffer(vertices);
}