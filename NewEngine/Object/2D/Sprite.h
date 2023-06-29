#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "TextureManager.h"
#include "Enum.h"
#include <vector>

class TextureAnimeiton;

class Sprite
{
private:
	std::vector<VertexBufferData::VSprite> vertices_;
	std::unique_ptr<VertexBuffer<VertexBufferData::VSprite>> vertexBuffer_;
	std::unique_ptr<Material> material_;
	GraphicsPipeline* graphicsPipeline_;
	Transform transform_;
	Transform* parent_;
	Texture* texture_;
	Vec2 anchorPoint_;
	Vec2 size_;
	FlipType flipType_;

public:
	Vec2 pos;
	Vec2 scale;
	float rot;
	Color color;

private: // マテリアル関連
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

private:
	void TransferVertexCoord();
	void TransferUVCoord(const Vec2 leftTopPos, const Vec2 rightDownPos);

public:
	Sprite();
	void Update(Transform* parent = nullptr);
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

public: //セッター

	// テクスチャー
	void SetTexture(Texture* texture);

	// 描画範囲
	void SetTextureRect(const Vec2 leftTopPos, const Vec2 rightDownPos);

	//　サイズ
	void SetSize(const Vec2 size);

	// アンカーポイント
	void SetAnchorPoint(const Vec2 anchorPoint);

	// 画像反転
	void SetFlipType(const FlipType flipType);

	// グラフィックスパイプライン
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

	// ブレンド
	void SetBlendMode(const BlendMode blendMode);

private:
	friend TextureAnimeiton;
};

