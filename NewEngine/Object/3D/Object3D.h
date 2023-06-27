#pragma once
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Model.h"
#include "Texture.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include <memory>

class Object3D
{
private:
	// バッファ転送
	void TransferBuffer();

	//  ブレンド設定
	void SetBlendMode(const BlendMode blendMode);

private:
	Vec3 worldPos_;
	Vec3 worldScale_;
	Transform transform_;
	Transform* parent_;
	Model* model_;
	Texture* texture_;
	Material material_;
	GraphicsPipeline* graphicsPipeline_;

	FbxAnimation animation;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;

	bool isLighting;
	static bool isAllLighting;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	Object3D();
	void Update(Transform* parent = nullptr);
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

public: //セッター

	// モデル
	void SetModel(Model* model);

	// テクスチャー
	void SetTexture(Texture* texture);

	// グラフィックスパイプライン
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

	// アニメーション
	void SetAnimation(const uint32_t animationIndex, const uint32_t maxFrame, const bool isPlay = true);

public: // ゲッター

	// ワールド座標
	Vec3 GetWorldPos();

	// ワールドスケール
	Vec3 GetWorldScale();

	// トランスフォーム
	Transform GetTransform();

	// モデル
	Model* GetModel();


};

