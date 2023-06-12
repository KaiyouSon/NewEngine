#include "CircleGaugeSprite.h"
#include "Camera.h"

using namespace ConstantBufferData;

CircleGaugeSprite::CircleGaugeSprite() :
	pos(0), scale(1), rot(0), anchorPoint(0.5f),
	startRadian(0), endRadian(0),
	vertexBuffer_(std::make_unique<VertexBuffer<VertexPosUv>>()),
	constantBufferTransform_(std::make_unique<ConstantBuffer<CTransform2D>>()),
	constantBufferColor_(std::make_unique<ConstantBuffer<CColor>>()),
	constantBufferCircleGauge_(std::make_unique<ConstantBuffer<CCircleGauge>>()),
	graphicsPipeline_(GraphicsPipelineManager::GetGraphicsPipeline("CircleGaugeSprite"))
{
	vertices_.resize(4);
	vertices_[0].uv = { 0.0f,1.0f };
	vertices_[1].uv = { 0.0f,0.0f };
	vertices_[2].uv = { 1.0f,1.0f };
	vertices_[3].uv = { 1.0f,0.0f };
	vertexBuffer_->Create(vertices_);

	// 定数バッファ
	constantBufferTransform_->Init();
	constantBufferColor_->Init();
	constantBufferCircleGauge_->Init();
}

void CircleGaugeSprite::Update()
{
	transform_.pos = pos;
	transform_.scale = { scale.x,scale.y,1 };
	transform_.rot = { 0,0,rot };
	transform_.Update();

	endRadian = Clamp<float>(endRadian, 0, Radian(360));

	// 定数バッファに転送
	constantBufferTransform_->constantBufferMap->mat =
		transform_.GetWorldMat() *
		Camera::current.GetOrthoGrphicProjectionMat();

	// 色転送
	constantBufferColor_->constantBufferMap->color = color / 255;
	constantBufferColor_->constantBufferMap->color.a = color.a / 255;

	// 色転送
	constantBufferCircleGauge_->constantBufferMap->startRadian = startRadian;
	constantBufferCircleGauge_->constantBufferMap->endRadian = endRadian + startRadian;

	// 頂点バッファーに頂点を転送
	TransferTexturePos();
}

void CircleGaugeSprite::Draw(const BlendMode blendMode)
{
	SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer_->GetvbViewAddress());

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform_->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferColor_->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, constantBufferCircleGauge_->constantBuffer->GetGPUVirtualAddress());

	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		renderBase->GetSpriteRootSignature()->GetRootDescriptorTableIndex(), texture->GetGpuHandle());

	renderBase->GetCommandList()->DrawInstanced((unsigned short)vertices_.size(), 1, 0, 0);
}

void CircleGaugeSprite::SetBlendMode(const BlendMode blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();//.get();

	switch (blendMode)
	{
	case BlendMode::Alpha: // αブレンド
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetAlphaPipeline());
		break;

	case BlendMode::Add:	// 加算ブレンド
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetAddPipeline());
		break;

	case BlendMode::Sub:	// 減算ブレンド
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetSubPipeline());
		break;

	case BlendMode::Inv:	// 反転
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetInvPipeline());
		break;

	default:
		break;
	}
}

void CircleGaugeSprite::TransferTexturePos()
{
	// 新しいのサイズ
	float width = texture->size.x;
	float height = texture->size.y;

	// 現在のサイズ
	float width2 = vertices_[0].pos.x - vertices_[2].pos.x;
	float height2 = vertices_[1].pos.x - vertices_[3].pos.x;

	if (width != fabsf(width2) || width != fabsf(height2))
	{
		vertices_[0].pos = { (0.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //左下
		vertices_[1].pos = { (0.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //左上
		vertices_[2].pos = { (1.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //右下
		vertices_[3].pos = { (1.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //右上

		vertexBuffer_->TransferToBuffer(vertices_);
	}
}