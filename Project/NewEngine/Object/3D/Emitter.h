#pragma once
#include "Transform.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "ParticleParam.h"

class Emitter
{
private:
	std::vector<VertexBufferData::VParticle> vertices_;
	std::unique_ptr<VertexBuffer<VertexBufferData::VParticle>> vertexBuffer_;
	Vec3 worldPos_;
	Vec3 worldScale_;
	Transform transform_;
	Transform* parent_;
	Texture* texture_;
	Material material_;
	GraphicsPipeline* graphicsPipeline_;
	Billboard billboard_;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;
	Vec2 tiling;
	Vec2 offset;
	std::vector<ParticleParameter::PParam0> pParam;
	uint32_t pSize;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

private:
	//  ブレンド設定
	void SetBlendMode(const BlendMode blendMode);

public:
	Emitter();
	void Update(Transform* parent = nullptr);
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

public: //セッター

	// テクスチャー
	void SetTexture(Texture* texture);

	// グラフィックスパイプライン
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

	// パーティクルの数
	void SetMaxParticle(const uint32_t max);

public: // ゲッター

	// ワールド座標
	Vec3 GetWorldPos();

	// ワールドスケール
	Vec3 GetWorldScale();

	// トランスフォーム
	Transform GetTransform();

};

