#pragma once
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Model.h"
#include "Texture.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include <memory>

class Object3D
{
private:
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CTransform3D>> constantBufferTransform_;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CMaterial>> constantBufferMaterial_;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CColor>> constantBufferColor_;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CSkin>> constantBufferSkin_;

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
	Color color_;
	GraphicsPipeline* graphicsPipeline_;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;

	bool isLighting;
	static bool isAllLighting;

public:
	Object3D();
	Object3D(Model* model);
	void PlayAnimetion();
	void Update(Transform* parent = nullptr);
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

#pragma region セッター

	// モデル
	void SetModel(Model* model);

	// テクスチャー
	void SetTexture(Texture* texture);

	// グラフィックスパイプライン
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

	// 色
	void SetColor(const Color color);

#pragma endregion

#pragma region ゲッター

	// ワールド座標
	Vec3 GetWorldPos();

	// ワールドスケール
	Vec3 GetWorldScale();

	// トランスフォーム
	Transform GetTransform();

	// モデル
	Model* GetModel();

#pragma endregion

};

