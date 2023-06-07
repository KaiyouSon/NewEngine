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
	std::unique_ptr<ConstantBuffer<ConstantBufferDataTransform3D>> constantBufferTransform_;
	std::unique_ptr<ConstantBuffer<ConstantBufferDataMaterial>> constantBufferMaterial_;
	std::unique_ptr<ConstantBuffer<ConstantBufferDataColor>> constantBufferColor_;
	std::unique_ptr<ConstantBuffer<ConstantBufferDataSkin>> constantBufferSkin_;

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
	Vec3 pos_;
	Vec3 scale_;
	Vec3 rot_;

	bool isLighting_;
	static bool isAllLighting_;

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

