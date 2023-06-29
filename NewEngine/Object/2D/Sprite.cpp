#include "Sprite.h"
#include "MathUtil.h"
#include "RenderBase.h"
#include "Camera.h"

using namespace VertexBufferData;
using namespace ConstantBufferData;

Sprite::Sprite() :
	texture_(TextureManager::GetTexture("White")),
	pos(0), scale(1), rot(0), color(Color::white), anchorPoint_(0.5f),
	vertexBuffer_(std::make_unique<VertexBuffer<VSprite>>()),
	material_(std::make_unique<Material>()),
	graphicsPipeline_(GraphicsPipelineManager::GetGraphicsPipeline("Sprite"))
{
	vertices_.resize(4);
	vertices_[0].uv = { 0.0f,1.0f };
	vertices_[1].uv = { 0.0f,0.0f };
	vertices_[2].uv = { 1.0f,1.0f };
	vertices_[3].uv = { 1.0f,0.0f };
	vertexBuffer_->Create(vertices_);

	// マテリアルの初期化
	MaterialInit();
}

void Sprite::Update(Transform* parent)
{
	transform_.pos = pos;
	transform_.scale = { scale.x,scale.y,1 };
	transform_.rot = { 0,0,rot };
	transform_.Update();

	if (parent != nullptr)
	{
		parent_ = parent;

		Mat4 mat = transform_.GetWorldMat();
		mat *= parent_->GetWorldMat();
		transform_.SetWorldMat(mat);
	}

	// マテリアルの転送
	MaterialTransfer();


	vertexBuffer_->TransferToBuffer(vertices_);
}
void Sprite::Draw(const BlendMode blendMode)
{
	SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer_->GetvbViewAddress());

	// マテリアルの描画コマンド
	MaterialDrawCommands();

	size_t max = renderBase->GetSpriteRootSignature()->GetConstantBufferNum();

	for (int i = 0; i < 1; i++)
	{
		// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
			UINT(max + i), texture_->GetGpuHandle());
	}

	renderBase->GetCommandList()->DrawInstanced((uint16_t)vertices_.size(), 1, 0, 0);
}

// --- マテリアル関連 --------------------------------------------------- //
void Sprite::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform2D>>();
	material_->constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	material_->constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	material_->Init();
}
void Sprite::MaterialTransfer()
{
	// マトリックス
	CTransform2D transform2DData =
	{
		transform_.GetWorldMat() * Camera::current.GetOrthoGrphicProjectionMat()
	};
	TransferDataToConstantBuffer(material_->constantBuffers[0].get(), transform2DData);

	// 色データ
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(material_->constantBuffers[1].get(), colorData);
}
void Sprite::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, material_->constantBuffers[0]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, material_->constantBuffers[1]->constantBuffer->GetGPUVirtualAddress());
}

// --- 頂点データ関連 --------------------------------------------------- //
void Sprite::TransferVertexCoord()
{
	enum class Point { LD, LU, RD, RU };

	// 四辺
	float left = (0.f - anchorPoint_.x) * size_.x;
	float right = (1.f - anchorPoint_.x) * size_.x;
	float up = (0.f - anchorPoint_.y) * size_.y;
	float down = (1.f - anchorPoint_.y) * size_.y;

	// 頂点座標
	vertices_[(uint32_t)Point::LD].pos = Vec3(left, down, 0.f);	  //左下
	vertices_[(uint32_t)Point::LU].pos = Vec3(left, up, 0.f);	  //左上
	vertices_[(uint32_t)Point::RD].pos = Vec3(right, down, 0.f);  //右下
	vertices_[(uint32_t)Point::RU].pos = Vec3(right, up, 0.f);	  //右上
}
void Sprite::TransferUVCoord(const Vec2 leftTopPos, const Vec2 rightDownPos)
{
	enum class Point { LD, LU, RD, RU };

	// 四辺
	float left = leftTopPos.x / texture_->size.x;
	float right = rightDownPos.x / texture_->size.x;
	float up = leftTopPos.y / texture_->size.y;
	float down = rightDownPos.x / texture_->size.y;

	// uv座標
	vertices_[(uint32_t)Point::LD].uv = Vec2(left, down);	 //左下
	vertices_[(uint32_t)Point::LU].uv = Vec2(left, up);		 //左上
	vertices_[(uint32_t)Point::RD].uv = Vec2(right, down);	 //右下
	vertices_[(uint32_t)Point::RU].uv = Vec2(right, up);	 //右上
}

// --- セッター -------------------------------------------------------- //

// テクスチャー
void Sprite::SetTexture(Texture* texture) { texture_ = texture; SetSize(texture->size); }

// 描画範囲
void Sprite::SetTextureRect(const Vec2 leftTopPos, const Vec2 rightDownPos)
{
	TransferUVCoord(leftTopPos, rightDownPos);
	vertexBuffer_->TransferToBuffer(vertices_);
}

// サイズ
void Sprite::SetSize(const Vec2 size)
{
	size_ = size;

	TransferVertexCoord();
	vertexBuffer_->TransferToBuffer(vertices_);
}

// アンカーポイント
void Sprite::SetAnchorPoint(const Vec2 anchorPoint)
{
	anchorPoint_ = anchorPoint;

	TransferVertexCoord();
	vertexBuffer_->TransferToBuffer(vertices_);
}

// ブレンド
void Sprite::SetBlendMode(const BlendMode blendMode)
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

// グラフィックスパイプライン
void Sprite::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	graphicsPipeline_ = graphicsPipeline;
}
