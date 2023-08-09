#pragma once
#include "Transform.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include "Camera.h"

class Object3D
{
private:
	Vec3 mWorldPos;
	Vec3 mWorldScale;
	Transform mTransform;
	Transform* mParent;
	Model* mModel;
	Texture* mTexture;
	Texture* mDissolveTex;
	Material mMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	Camera* mCamera;
	bool mIsWriteShadow;
	bool mIsWriteDepth;

private:
	// 後整理する
	Texture* mWhiteTex;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;
	Vec2 tiling;
	Vec2 offset;

	float dissolve;
	float colorPower;
	Color dissolveColor;


	bool isLighting;
	bool isUseDissolve;
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

	// カメラ
	void SetCamera(Camera* camera = nullptr);

	// 影
	void SetisShadow(const bool isWriteShadow, const bool isWriteDepth);

	// 親
	void SetParent(Transform* parent);

public: // ゲッター

	// ワールド座標
	Vec3 GetWorldPos();

	// ワールドスケール
	Vec3 GetWorldScale();

	// トランスフォーム
	Transform GetTransform();

	// 親のトランスフォーム
	Transform* GetParent();

	// モデル
	Model* GetModel();
};

